/**
 * @file    inheritance_priv.h
 * @author  Cypherock X1 Team
 * @brief   Support for inheritance app internal operations
 *          This file is defined to separate INHERITANCE's internal use
 * functions, flows, common APIs
 * @copyright Copyright (c) 2023 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 * target=_blank>https://mitcc.org/</a>
 */
#ifndef INHERITANCE_PRIV_H
#define INHERITANCE_PRIV_H
/*****************************************************************************
 * INCLUDES
 *****************************************************************************/
#include <stdint.h>

#include "ed25519.h"
#include "inheritance/core.pb.h"
#include "wallet.h"

/*****************************************************************************
 * PRIVATE MACROS AND DEFINES
 *****************************************************************************/
#define CHALLENGE_SIZE_MAX 32
#define CHALLENGE_SIZE_MIN 16
#define ENTROPY_SIZE_LIMIT 100
#define SIZE_SEED 64
/*****************************************************************************
 * PRIVATE TYPEDEFS
 *****************************************************************************/
typedef enum {
  AUTH_WALLET_OK = 0,
  AUTH_WALLET_TYPE_INVALID,      // TODO: In inheriance app - Show error message
                                 // on ui that which assert is wrong
  AUTH_WALLET_INPUTS_INVALID,    // TODO: In inheritance app - Add in wallet id
                                 // comparison, show error message on ui
} auth_wallet_error_type_e;

#pragma pack(push, 1)
typedef struct {
  ed25519_signature signature;
  ed25519_public_key public_key;
} auth_wallet_result_t;

typedef struct {
  uint8_t wallet_id[WALLET_ID_SIZE];
  uint8_t challenge[CHALLENGE_SIZE_MAX];
  size_t challenge_size;
} auth_wallet_data_t;

typedef struct {
  const uint8_t entropy[ENTROPY_SIZE_LIMIT];
  uint8_t entropy_size;
  ed25519_secret_key private_key;
  auth_wallet_result_t result;
  bool has_data;
} auth_wallet_private_t;

typedef struct {
  auth_wallet_data_t data;
  auth_wallet_private_t wallet_based_data;
  auth_wallet_private_t seed_based_data;
  bool with_public_key;
  bool do_seed_based;
  bool do_wallet_based;
  auth_wallet_error_type_e status;
} auth_wallet_config_t;
#pragma pack(pop)

/*****************************************************************************
 * TYPEDEFS
 *****************************************************************************/

/*****************************************************************************
 * EXPORTED VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * @brief Entry point to auth wallet flow
 *
 */
void inheritance_auth_wallet(inheritance_query_t *query);

#endif /* INHERITANCE_PRIV_H */
