/**
 * @file    icp_context.h
 * @author  Cypherock X1 Team
 * @brief   Header file defining typedefs and MACROS for the ICP app
 * @copyright Copyright (c) 2024 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 * target=_blank>https://mitcc.org/</a>
 */
#ifndef ICP_CONTEXT_H
#define ICP_CONTEXT_H

/*****************************************************************************
 * INCLUDES
 *****************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/*****************************************************************************
 * MACROS AND DEFINES
 *****************************************************************************/
#define ICP_NAME "ICP"
#define ICP_LUNIT "ICP"
#define ICP_PUB_KEY_SIZE 33
#define ICP_PRINCIPAL_LENGTH 29
#define ICP_ACCOUNT_ID_LENGTH 32

#define ICP_SELF_AUTH_ID_TAG 0x02
#define SECP256K1_UNCOMPRESSED_PK_LEN 65

#define ICP_SUBACCOUNT_ID_LEN 32

/*****************************************************************************
 * TYPEDEFS
 *****************************************************************************/

// TODO: Populate structure for ICP
typedef struct {
} icp_config_t;

typedef enum {
  Null = -1,
  Bool = -2,
  Nat = -3,
  Int = -4,

  Nat8 = -5,
  Nat16 = -6,
  Nat32 = -7,
  Nat64 = -8,

  Int8 = -9,
  Int16 = -10,
  Int32 = -11,
  Int64 = -12,

  Float32 = -13,
  Float64 = -14,
  Text = -15,
  Reserved = -16,
  Empty = -17,
  Opt = -18,
  Vector = -19,
  Record = -20,
  Variant = -21,
  Func = -22,
  Service = -23,
  Principal = -24,
} IDLTypes_e;

typedef enum {
  transfer_hash_to = 25979,
  transfer_hash_fee = 5094982,
  transfer_hash_memo = 1213809850,
  transfer_hash_from_subaccount = 1835347746,    // optional
  transfer_hash_timestamp = 3258775938,          // optional
  transfer_hash_amount = 3573748184,
} transfer_hash_fields;

typedef struct {
  uint64_t e8s;
} token_t;

typedef struct {
  uint8_t to[ICP_ACCOUNT_ID_LENGTH];
  token_t *amount;
  token_t *fee;
  uint64_t memo;
} icp_transfer_t;

/*****************************************************************************
 * EXPORTED VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *****************************************************************************/

#endif /* ICP_CONTEXT_H */
