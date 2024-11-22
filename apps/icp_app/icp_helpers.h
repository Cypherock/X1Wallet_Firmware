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

/*****************************************************************************
 * MACROS AND DEFINES
 *****************************************************************************/

#define ICP_IMPLICIT_ACCOUNT_DEPTH 5

#define ICP_PURPOSE_INDEX 0x8000002C         // 44'
#define ICP_COIN_INDEX (0x80000000 | 223)    // 223'
#define ICP_ACCOUNT_INDEX 0x80000000         // 0'
#define ICP_CHANGE_INDEX 0x00000000          // 0

/*****************************************************************************
 * TYPEDEFS
 *****************************************************************************/

#define ICP_PREFIXED_ACCOUNT_ID_LENGTH 21
#define ICP_ACCOUNT_ADDRESS_LENGTH 34
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
 * key.
 *
 * This function encodes the provided uncompressed public key in DER format and
 * stores the result in the `result` buffer.
 *
 * @param[out] result The buffer to store the DER-encoded public key.
 * @param[in] public_key The uncompressed public key to encode in DER format.
 *
 * @note The caller must ensure that `result` has sufficient space to store the
 * DER-encoded public key.
 *
 * @return None
 */
void get_der_encoded_pub_key(uint8_t *result, const uint8_t *public_key);

/**
 * @brief Updates the CRC32 checksum with a new byte of data.
 *
 * This function takes the current CRC32 value and updates it with the provided
 * byte. The CRC32 algorithm is typically used for error-checking or generating
 * a checksum for a stream of data.
 *
 * @param[in] crc_in The current CRC32 checksum value.
 * @param[in] byte The byte to include in the CRC32 update.
 *
 * @return The updated CRC32 checksum value.
 */
uint32_t update_crc32(uint32_t crc_in, uint8_t byte);

/**
 * @brief Computes the CRC32 checksum for a given input buffer.
 *
 * This function computes the CRC32 checksum of the provided input data. The
 * checksum is a 32-bit value commonly used for data integrity checks.
 *
 * @param[in] input The input data for which the CRC32 checksum is computed.
 * @param[in] input_size The size of the input data in bytes.
 *
 * @return The CRC32 checksum of the input data.
 */
uint32_t get_crc32(const uint8_t *input, size_t input_size);

#endif    // ICP_HELPERS_H
