/**
 * @file    icp_helpers.c
 * @author  Cypherock X1 Team
 * @brief   Utilities specific to Icp chains
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
#include "icp_helpers.h"

#include <stddef.h>
#include <stdint.h>

#include "sha2.h"

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
 * STATIC FUNCTION PROTOTYPES
 *****************************************************************************/

/*****************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/
#define CRC32_POLYNOMIAL                                                       \
  0xEDB88320                     // Reversed polynomial used in standard CRC32
#define CRC32_INIT 0xFFFFFFFF    // Initial value
#define CRC32_FINAL_XOR 0xFFFFFFFF    // Final XOR value

// Secp256k1 OID (Object Identifier) Prefix
uint8_t const SECP256K1_DER_PREFIX[] = {
    0x30, 0x56, 0x30, 0x10, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02,
    0x01, 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x0a, 0x03, 0x42, 0x00};

// Precomputed CRC-32 table
uint32_t crc32_table[256];

// SHA-224 Initial Hash Values (IV) from FIPS 180-4
static const uint32_t sha224_initial_hash[8] = {0xc1059ed8,
                                                0x367cd507,
                                                0x3070dd17,
                                                0xf70e5939,
                                                0xffc00b31,
                                                0x68581511,
                                                0x64f98fa7,
                                                0xbefa4fa4};

/*****************************************************************************
 * STATIC FUNCTIONS
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTIONS
 *****************************************************************************/

bool icp_derivation_path_guard(const uint32_t *path, uint8_t levels) {
  bool status = false;
  if (levels != ICP_IMPLICIT_ACCOUNT_DEPTH) {
    return status;
  }

  uint32_t purpose = path[0], coin = path[1], account = path[2],
           change = path[3], address = path[4];

  // m/44'/223'/0'/0/i
  status = (ICP_PURPOSE_INDEX == purpose && ICP_COIN_INDEX == coin &&
            ICP_ACCOUNT_INDEX == account && ICP_CHANGE_INDEX == change &&
            is_non_hardened(address));

  return status;
}

void get_secp256k1_der_encoded_pub_key(const uint8_t *public_key,
                                       uint8_t *result) {
  memzero(result, SECP256K1_DER_PK_LEN);
  memcpy(result, SECP256K1_DER_PREFIX, SECP256K1_DER_PREFIX_LEN);
  memcpy(result + SECP256K1_DER_PREFIX_LEN,
         public_key,
         SECP256K1_UNCOMPRESSED_PK_LEN);
}

// Function to initialize the CRC-32 lookup table
void crc32_init() {
  for (uint32_t i = 0; i < 256; i++) {
    uint32_t crc = i;
    for (uint32_t j = 0; j < 8; j++) {
      if (crc & 1)
        crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
      else
        crc >>= 1;
    }
    crc32_table[i] = crc;
  }
}

uint32_t compute_crc32(const uint8_t *data, size_t length) {
  // Initialize CRC32 table
  crc32_init();

  uint32_t crc = CRC32_INIT;
  for (size_t i = 0; i < length; i++) {
    uint8_t index = (crc ^ data[i]) & 0xFF;
    crc = (crc >> 8) ^ crc32_table[index];
  }
  return crc ^ CRC32_FINAL_XOR;
}

// Custom SHA-224 function using SHA-256 core
void sha224_Raw(const uint8_t *data, size_t len, uint8_t *digest) {
  SHA256_CTX ctx;
  sha256_Init(&ctx);

  // Override initial hash values with SHA-224 IV
  memcpy(ctx.state, sha224_initial_hash, sizeof(sha224_initial_hash));

  sha256_Update(&ctx, data, len);
  sha256_Final(&ctx, digest);

  // Truncate to 224 bits (first 28 bytes)
  memzero(&digest[SHA224_DIGEST_LENGTH], 32 - SHA224_DIGEST_LENGTH);
}