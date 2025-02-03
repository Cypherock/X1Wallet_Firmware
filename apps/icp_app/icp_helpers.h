/**
 * @file    icp_helpers.h
 * @author  Cypherock X1 Team
 * @brief   Utilities api definitions for ICP chains
 * @copyright Copyright (c) 2024 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 * target=_blank>https://mitcc.org/</a>
 */
#ifndef ICP_HELPERS_H
#define ICP_HELPERS_H

/*****************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "coin_utils.h"
#include "icp_context.h"

/*****************************************************************************
 * MACROS AND DEFINES
 *****************************************************************************/

#define ICP_IMPLICIT_ACCOUNT_DEPTH 5

#define ICP_PURPOSE_INDEX 0x8000002C         // 44'
#define ICP_COIN_INDEX (0x80000000 | 223)    // 223'
#define ICP_ACCOUNT_INDEX 0x80000000         // 0'
#define ICP_CHANGE_INDEX 0x00000000          // 0

#define ICP_PREFIXED_ACCOUNT_ID_LENGTH 21
#define ICP_ACCOUNT_ADDRESS_LENGTH 34
#define SECP256K1_DER_PREFIX_LEN                                               \
  23    // Secp256k1 OID (Object Identifier) prefix len
#define SECP256K1_DER_PK_LEN                                                   \
  SECP256K1_DER_PREFIX_LEN + SECP256K1_UNCOMPRESSED_PK_LEN

#define SHA224_DIGEST_LENGTH 28

/*****************************************************************************
 * TYPEDEFS
 *****************************************************************************/\
/*****************************************************************************
 * EXPORTED VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * @brief Verifies the derivation path.
 * @details The derivation depth is fixed at level 5. So if the depth level !=
 * 5, then this function return false indicating invalid derivation path. The
 * function supports checking derivation paths for HD wallets Types of
 * derivations
 *
 * @param[in] path      The derivation path as an uint32 array
 * @param[in] levels     The number of levels in the derivation path
 *
 * @return bool Indicates if the provided derivation path is valid
 * @retval true if the derivation path is valid
 * @retval false otherwise
 */
bool icp_derivation_path_guard(const uint32_t *path, uint8_t levels);

/**
 * @brief Derives the DER-encoded public key from the given uncompressed public
 * key for Secp256k1.
 *
 * This function encodes the provided uncompressed public key in DER format and
 * stores the result in the `result` buffer.
 *
 * @param[in] public_key The uncompressed public key to encode in DER format.
 * @param[out] result The buffer to store the DER-encoded public key.
 *
 * @note The caller must ensure that `result` has sufficient space to store the
 * DER-encoded public key.
 *
 * @return None
 */
void get_secp256k1_der_encoded_pub_key(const uint8_t *public_key,
                                       uint8_t *result);

/**
 * @brief Computes the CRC32 checksum for a given input buffer.
 *
 * This function computes the CRC32 checksum of the provided input data using a
 * lookup table. The checksum is a 32-bit value commonly used for data integrity
 * checks.
 *
 * @param[in] data The input data for which the CRC32 checksum is computed.
 * @param[in] length The length of the input data in bytes.
 *
 * @return The CRC32 checksum of the input data.
 */
uint32_t compute_crc32(const uint8_t *data, size_t length);

/**
 * @brief Custom SHA-224 function using SHA-256 core.
 *
 * This function hashes the data using SHA-224 algorithm.
 *
 * @param[in] data The input data to hash.
 * @param[in] len The length of the input data.
 * @param[out] digest The buffer to store the hash digest.
 *
 * @note The caller must ensure that `digest` has sufficient space to store the
 * hash digest
 *
 * @return None.
 */
void sha224_Raw(const uint8_t *data, size_t len, uint8_t *digest);

#endif    // ICP_HELPERS_H
