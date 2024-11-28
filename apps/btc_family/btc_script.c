/**
 * @file    btc_script.c
 * @author  Cypherock X1 Team
 * @brief   Bitcoin scriptPubKey utility apis.
 * @copyright Copyright (c) 2023 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 *target=_blank>https://mitcc.org/</a>
 *
 ******************************************************************************
 * @attention
 *
 * (c) Copyright 2023 by HODL TECH PTE LTD
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

#include "btc_script.h"

#include "base58.h"
#include "bip32.h"
#include "btc_priv.h"
#include "memzero.h"
#include "ripemd160.h"
#include "segwit_addr.h"

/*****************************************************************************
 * EXTERN VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * PRIVATE MACROS AND DEFINES
 *****************************************************************************/

#define BTC_PUB_KEY_SIZE 65
#define BTC_SHORT_PUB_KEY_SIZE 33

/*****************************************************************************
 * PRIVATE TYPEDEFS
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *****************************************************************************/

/*****************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTIONS
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTIONS
 *****************************************************************************/

bool btc_is_p2pk(const uint8_t *script, size_t script_len) {
  bool status = false;
  if ((BTC_PUB_KEY_SIZE + 2) == script_len && BTC_PUB_KEY_SIZE == script[0] &&
      OP_CHECKSIG == script[BTC_PUB_KEY_SIZE + 1]) {
    status = true;
  }
  if ((BTC_SHORT_PUB_KEY_SIZE + 2) == script_len &&
      BTC_SHORT_PUB_KEY_SIZE == script[0] &&
      OP_CHECKSIG == script[BTC_SHORT_PUB_KEY_SIZE + 1]) {
    status = true;
  }
  return status;
}

btc_script_type_e btc_get_script_type(const uint8_t *script,
                                      size_t script_len) {
  if (true == btc_is_p2pk(script, script_len)) {
    return SCRIPT_TYPE_P2PK;
  }

  if (true == btc_is_p2pkh(script, script_len)) {
    return SCRIPT_TYPE_P2PKH;
  }

  if (true == btc_is_p2sh(script, script_len)) {
    return SCRIPT_TYPE_P2SH;
  }

  if (true == btc_is_p2wpkh(script, script_len)) {
    return SCRIPT_TYPE_P2WPKH;
  }

  if (true == btc_is_p2wsh(script, script_len)) {
    return SCRIPT_TYPE_P2WSH;
  }

  if (true == btc_is_p2tr(script, script_len)) {
    return SCRIPT_TYPE_P2TR;
  }

  // check if it is a potentially valid future segwit scriptPubKey. Refer
  // BIP-0141
  if (script_len >= 4 && script_len <= 42 &&
      (script[0] == 0 || (script[0] >= OP_1 && script[0] <= OP_16))) {
    uint8_t push_len = script[1];
    if (script_len == 1 + 1 + push_len) {
      return SCRIPT_TYPE_UNKNOWN_SEGWIT;
    }
  }

  if (true == btc_is_p2ms(script, script_len)) {
    return SCRIPT_TYPE_P2MS;
  }

  if (true == btc_is_opreturn(script, script_len)) {
    return SCRIPT_TYPE_NULL_DATA;
  }

  // unknown/non-standard, or doesn't have an address
  return SCRIPT_TYPE_NONSTANDARD;
}

int btc_get_script_pub_address(const uint8_t *script,
                               size_t script_len,
                               char *addr,
                               int out_len) {
  btc_script_type_e type = btc_get_script_type(script, script_len);
  int status = -3;

  switch (type) {
    case SCRIPT_TYPE_P2PKH:
    case SCRIPT_TYPE_P2SH: {
      uint8_t raw_addr[21] = {0};
      int offset = (SCRIPT_TYPE_P2PKH == type) ? 3 : 2;
      raw_addr[0] = (SCRIPT_TYPE_P2PKH == type) ? g_btc_app->p2pkh_addr_ver
                                                : g_btc_app->p2sh_addr_ver;
      memcpy(&raw_addr[1], &script[offset], 20);
      status =
          base58_encode_check(raw_addr, 21, HASHER_SHA2D, addr, out_len - 1);
      if (0 == status) {
        // base58_encode_check returns 0 on failure
        status = -3;
      }
      break;
    }

    case SCRIPT_TYPE_P2WPKH:
    case SCRIPT_TYPE_P2WSH:
    case SCRIPT_TYPE_P2TR:
    case SCRIPT_TYPE_UNKNOWN_SEGWIT: {
      if (out_len < 73 + BECH32_HRP_SIZE) {
        return -3;
      }

      int version = (script[0] == 0 ? 0 : script[0] - 0x50);
      // get null-terminated string output address
      status = segwit_addr_encode(
          addr, g_btc_app->bech32_hrp, version, script + 2, script[1]);

      if (status != 1) {
        // segwit_addr_encode returns 0 on failure and 1 on success
        status = -3;
      } else {
        // practically cannot overflow
        status = (int)strnlen(addr, out_len);
      }
      break;
    }

    case SCRIPT_TYPE_P2PK:
    case SCRIPT_TYPE_P2MS:
    case SCRIPT_TYPE_NULL_DATA:
      status = -1;
      break;

    case SCRIPT_TYPE_NONSTANDARD:
    default:
      status = -2;
      break;
  }
  if (status >= 0) {
    addr[status] = '\0';
  }
  return status;
}

bool btc_check_script_address(const uint8_t *script,
                              size_t script_len,
                              const uint8_t *public_key) {
  if (NULL == public_key || NULL == script) {
    return false;
  }

  uint8_t digest[HASHER_DIGEST_LENGTH] = {0};
  btc_script_type_e type = btc_get_script_type(script, script_len);
  if (SCRIPT_TYPE_P2PKH != type && SCRIPT_TYPE_P2WPKH != type &&
      SCRIPT_TYPE_P2SH != type) {
    // allow only p2pkh and p2wpkh and p2sh-p2wpkh for change output
    return false;
  }
  uint8_t offset = (SCRIPT_TYPE_P2PKH == type) ? 3 : 2;

  hasher_Raw(HASHER_SHA2_RIPEMD, public_key, BTC_SHORT_PUB_KEY_SIZE, digest);

  if (SCRIPT_TYPE_P2SH == type) {
    // Compute redeemscript(P2WPKH). scriptpub of nested-segwit is hash160 of
    // redeemscript
    uint8_t buf[22] = {0};
    buf[0] = 0;     // version byte
    buf[1] = 20;    // push 20 bytes
    memcpy(buf + 2, digest, 20);
    hasher_Raw(HASHER_SHA2_RIPEMD, buf, 22, digest);
  }

  return (memcmp(digest, &script[offset], RIPEMD160_DIGEST_LENGTH) == 0);
}

uint8_t btc_sig_to_script_sig(const uint8_t *sig,
                              const uint8_t *pub_key,
                              uint8_t *script_sig) {
  uint8_t script_sig_len = 0;
  if (NULL == sig || NULL == pub_key || NULL == script_sig) {
    return script_sig_len;
  }

  uint8_t script[128] = {0};
  memzero(script, sizeof(script));
  uint8_t der_sig_len = ecdsa_sig_to_der(sig, &script[1]);

  // PUSHDATA Opcode(1) + der_sig_len + SigHash Code(1) + PUSHDATA Opcode(1) +
  // Public Key(33)
  script_sig_len = 1 + der_sig_len + 2 + 33;
  script[0] = der_sig_len + 1;
  script[1 + der_sig_len] = 1;         // sighash code: 1
  script[1 + der_sig_len + 1] = 33;    // push data opcode: 33
  memcpy(&script[1 + der_sig_len + 1 + 1], pub_key, 33);
  memcpy(script_sig, script, 128);
  return script_sig_len;
}
