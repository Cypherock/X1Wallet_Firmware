/**
 * @file    icp_txn_helpers.c
 * @author  Cypherock X1 Team
 * @brief   Helper functions for the ICP app for txn signing flow
 * @copyright Copyright (c) 2024 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 *target=_blank>https://mitcc.org/</a>
 *
 ******************************************************************************
 * @attention
 *
 * (c) Copyright 2024 by HODL TECH PTE LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * "Commons Clause" License Condition v1.0
 *
 * The Software is provided to you by the Licensor under the License,
 * as defined below, subject to the following condition.
 *
 * Without limiting other conditions in the License, the grant of
 * rights under the License will not include, and the License does not
 * grant to you, the right to Sell the Software.
 *
 * For purposes of the foregoing, "Sell" means practicing any or all
 * of the rights granted to you under the License to provide to third
 * parties, for a fee or other consideration (including without
 * limitation fees for hosting or consulting/ support services related
 * to the Software), a product or service whose value derives, entirely
 * or substantially, from the functionality of the Software. Any license
 * notice or attribution required by the License must also include
 * this Commons Clause License Condition notice.
 *
 * Software: All X1Wallet associated files.
 * License: MIT
 * Licensor: HODL TECH PTE LTD
 *
 ******************************************************************************
 */

/*****************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "icp_txn_helpers.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "icp_context.h"
#include "ui_core_confirm.h"
#include "utils.h"

/*****************************************************************************
 * EXTERN VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * PRIVATE MACROS AND DEFINES
 *****************************************************************************/

/*****************************************************************************
 * PRIVATE TYPEDEFS
 *****************************************************************************/

/*****************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTIONS
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTIONS
 *****************************************************************************/

/**
 * Hash a string using SHA-256.
 */
void hash_string(const char *value, uint8_t *hash) {
  sha256_Raw((const uint8_t *)value, strlen(value), hash);
}

/**
 * Hash a given LEB128-encoded integer.
 */
void hash_leb128(uint64_t value, uint8_t *hash) {
  uint8_t buffer[10];
  size_t offset = 0;

  do {
    buffer[offset] = (value & 0x7F) | (value >= 0x80 ? 0x80 : 0);
    value >>= 7;
    offset++;
  } while (value > 0);

  sha256_Raw(buffer, offset, hash);
}

/**
 * Comparison function for sorting hash pairs.
 */
int compare_hashes(const void *a, const void *b) {
  return memcmp(((HashPair *)a)->key_hash, ((HashPair *)b)->key_hash, 32);
}

bool decode_vec(int64_t type,
                const uint8_t *data,
                size_t *offset,
                uint8_t *res) {
  switch (type) {
    case Nat8: {
      uint64_t len = leb_decode(data, offset);
      memcpy(res, data + *offset, len);
      (*offset) += len;
      break;
    }
    default:
      return false;
  }
  return true;
}

bool read_recipient_account_id(const uint8_t *data,
                               size_t *offset,
                               icp_transfer_t *txn) {
  return decode_vec(Nat8, data, offset, txn->to);
}

bool read_amount_value(const uint8_t *data,
                       size_t *offset,
                       icp_transfer_t *txn) {
  txn->amount = (token_t *)malloc(sizeof(token_t));
  txn->amount->e8s = U64_READ_LE_ARRAY(data + *offset);
  (*offset) += 8;
  return true;
}

bool read_fee_value(const uint8_t *data, size_t *offset, icp_transfer_t *txn) {
  txn->fee = (token_t *)malloc(sizeof(token_t));
  txn->fee->e8s = U64_READ_LE_ARRAY(data + *offset);
  (*offset) += 8;
  return true;
}

bool read_memo_value(const uint8_t *data, size_t *offset, icp_transfer_t *txn) {
  txn->memo = U64_READ_LE_ARRAY(data + *offset);
  (*offset) += 8;
  return true;
}

uint64_t leb_decode(const uint8_t *buffer, size_t *offset) {
  uint64_t result = 0;
  int shift = 0;
  uint8_t byte;
  do {
    byte = buffer[*offset];
    (*offset)++;
    result |= ((uint64_t)(byte & 0x7F) << shift);
    shift += 7;
  } while ((byte & 0x80) != 0);
  return result;
}

int64_t sleb_decode(const uint8_t *buffer, size_t *offset) {
  int64_t result = 0;
  int shift = 0;
  uint8_t byte;

  while (1) {
    byte = buffer[*offset];
    (*offset)++;
    result |= (int64_t)(byte & 0x7F) << shift;
    shift += 7;

    // If MSB is not set, break (last byte)
    if ((byte & 0x80) == 0) {
      // Check if the sign bit (0x40) is set for negative numbers
      if (shift < 64 && (byte & 0x40)) {
        result |= -((int64_t)1 << shift);
      }
      break;
    }
  }
  return result;
}

// @TODO: add unit tests for parser
bool icp_parse_transfer_txn(const uint8_t *byte_array,
                            uint16_t byte_array_size,
                            icp_transfer_t *txn) {
  size_t offset = 0;

  // Verify the "DIDL" Magic Number
  if (memcmp(byte_array, MAGIC_NUMBER, 4) != 0) {
    return false;
  }
  offset += 4;

  // Decode Type Table
  size_t num_types = leb_decode(byte_array, &offset);
  IDLComplexType type_table[num_types];

  for (size_t i = 0; i < num_types; i++) {
    int64_t type = sleb_decode(byte_array, &offset);

    switch (type) {
      case Opt:
      case Vector: {
        int64_t child_type = sleb_decode(byte_array, &offset);
        IDLComplexType c_ty;
        c_ty.type_id = type;
        c_ty.child_type = child_type;
        type_table[i] = c_ty;
        break;
      }
      case Record: {
        IDLComplexType c_ty;
        c_ty.type_id = type;
        c_ty.num_fields = leb_decode(byte_array, &offset);
        c_ty.fields =
            (RecordField *)malloc(sizeof(RecordField) * c_ty.num_fields);
        for (int j = 0; j < c_ty.num_fields; j++) {
          uint64_t hash = leb_decode(byte_array, &offset);
          int64_t field_type = sleb_decode(byte_array, &offset);
          RecordField field;
          field.key_hash = hash;
          field.type = field_type;
          c_ty.fields[j] = field;
        }

        type_table[i] = c_ty;
        break;
      }
      default:
        return false;
    }
  }

  uint64_t arg_count = leb_decode(byte_array, &offset);
  // only 1 argument supported
  if (arg_count != 1) {
    return false;
  }

  uint64_t arg_type_index = leb_decode(byte_array, &offset);

  if (arg_type_index < 0 && arg_type_index >= num_types) {
    return false;
  }

  IDLComplexType arg_type = type_table[arg_type_index];

  // we expect a single record of 4 fields (to, amount, fee, memo)
  if (arg_type.type_id != Record || arg_type.num_fields != 4) {
    return false;
  }

  // Decode Transfer Args
  for (int i = 0; i < arg_type.num_fields; i++) {
    RecordField field = arg_type.fields[i];
    switch (field.key_hash) {
      case transfer_hash_to:
        // we can also verify the type
        // not doing right now
        if (!read_recipient_account_id(byte_array, &offset, txn)) {
          return false;
        }
        break;
      case transfer_hash_amount:
        if (!read_amount_value(byte_array, &offset, txn)) {
          return false;
        }
        break;
      case transfer_hash_fee:
        if (!read_fee_value(byte_array, &offset, txn)) {
          return false;
        }
        break;
      case transfer_hash_memo:
        if (!read_memo_value(byte_array, &offset, txn)) {
          return false;
        }
        break;
      default:
        return false;
    }
  }

  if (offset != byte_array_size) {
    return false;
  }

  return true;
}

/**
 * Hash icp_transfer_request_t structure using SHA-256, sorting key-value pairs
 * first.
 */
void hash_icp_transfer_request(const icp_transfer_request_t *request,
                               size_t arg_size,
                               uint8_t *hash) {
  HashPair pairs[7];
  size_t pair_count = 0;

  hash_string("request_type", pairs[pair_count].key_hash);
  hash_string(request->request_type, pairs[pair_count].value_hash);
  pair_count++;

  hash_string("canister_id", pairs[pair_count].key_hash);
  sha256_Raw(request->canister_id,
             sizeof(request->canister_id),
             pairs[pair_count].value_hash);
  pair_count++;

  hash_string("method_name", pairs[pair_count].key_hash);
  hash_string(request->method_name, pairs[pair_count].value_hash);
  pair_count++;

  hash_string("sender", pairs[pair_count].key_hash);
  sha256_Raw(
      request->sender, sizeof(request->sender), pairs[pair_count].value_hash);
  pair_count++;

  hash_string("ingress_expiry", pairs[pair_count].key_hash);
  hash_leb128(request->ingress_expiry, pairs[pair_count].value_hash);
  pair_count++;

  hash_string("nonce", pairs[pair_count].key_hash);
  sha256_Raw(
      request->nonce, sizeof(request->nonce), pairs[pair_count].value_hash);
  pair_count++;

  hash_string("arg", pairs[pair_count].key_hash);
  sha256_Raw(request->arg, arg_size, pairs[pair_count].value_hash);
  pair_count++;

  // Sort key-value pairs by key hash
  qsort(pairs, pair_count, sizeof(HashPair), compare_hashes);

  uint8_t concatenated[512];
  size_t offset = 0;
  for (size_t i = 0; i < pair_count; i++) {
    memcpy(&concatenated[offset], pairs[i].key_hash, 32);
    offset += 32;
    memcpy(&concatenated[offset], pairs[i].value_hash, 32);
    offset += 32;
  }

  sha256_Raw(concatenated, offset, hash);
}