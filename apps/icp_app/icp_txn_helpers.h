/**
 * @file    icp_txn_helpers.h
 * @author  Cypherock X1 Team
 * @brief   Helper functions for the ICP app for txn signing flow
 * @copyright Copyright (c) 2024 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 * target=_blank>https://mitcc.org/</a>
 */
#ifndef ICP_TXN_HELPERS_H
#define ICP_TXN_HELPERS_H

/*****************************************************************************
 * INCLUDES
 *****************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "icp_context.h"
#include "icp_priv.h"

/*****************************************************************************
 * MACROS AND DEFINES
 *****************************************************************************/
#define MAGIC_NUMBER "DIDL"

/*****************************************************************************
 * TYPEDEFS
 *****************************************************************************/
typedef struct {
  uint64_t key_hash;
  int64_t type;
} RecordField;

typedef struct {
  int64_t type_id;

  // if it's a compound type example Vector(Nat8)
  int64_t child_type;

  // if it's a record type
  uint64_t num_fields;
  RecordField *fields;
} IDLComplexType;

typedef struct {
  uint8_t key_hash[32];
  uint8_t value_hash[32];
} HashPair;

/*****************************************************************************
 * EXPORTED VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * Function to decode LEB128 encoded positive integers.
 * @param buffer Pointer to the LEB128 encoded bytes.
 * @param offset Offset from the start of the buffer.
 * @return Decoded integer value.
 */
uint64_t leb_decode(const uint8_t *buffer, size_t *offset);

/**
 * Decode a Signed LEB128 encoded buffer into an integer.
 * Supports decoding of negative values using two's complement.
 * @param buffer Pointer to the LEB128 encoded bytes.
 * @param offset Offset from the start of the buffer.
 * @return Decoded integer value.
 */
int64_t sleb_decode(const uint8_t *buffer, size_t *offset);

/**
 * @brief Parse byte array of unsigned txn and store decoded information to be
 * used for user confirmation.
 * @details Only icp payment transaction is supported as of now.
 *
 * @param byte_array Constant reference to buffer containing the raw unsigned
 * txn
 * @param byte_array_size Size in bytes of the txn
 * @param utxn Reference to buffer where decoded information will be populated.
 * It can be used at a later stage for user verification.
 * @return true If the parsing was successful
 * @return false If the parsing failed - it could be due to an unsupported
 * transaction or data type or missing information
 */
bool icp_parse_transfer_txn(const uint8_t *byte_array,
                            uint16_t byte_array_size,
                            icp_transfer_t *utxn);

/**
 * @brief Hash icp_transfer_request_t structure using SHA-256.
 *
 * @param[in] request icp_transfer_request_t to hash
 * @param [out] hash The buffer to store the result hash
 *
 * @note The caller must ensure that `hash` has sufficient space to store the
 * hash digest
 *
 * @return None.
 */
void hash_icp_transfer_request(const icp_transfer_request_t *request,
                               uint8_t *hash);

#endif /* ICP_TXN_HELPERS_H */
