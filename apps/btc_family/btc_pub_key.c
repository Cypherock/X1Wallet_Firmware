/**
 * @file    btc_pub_key.c
 * @author  Cypherock X1 Team
 * @brief   Generates public key for bitcoin derivations.
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

#include "bip32.h"
#include "btc_api.h"
#include "btc_helpers.h"
#include "btc_priv.h"
#include "coin_utils.h"
#include "curves.h"
#include "reconstruct_wallet_flow.h"
#include "status_api.h"
#include "ui_core_confirm.h"
#include "ui_screens.h"
#include "wallet_list.h"

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

/**
 * @brief Checks if the provided query contains expected request.
 * @details The function performs the check on the request type and if the check
 * fails, then it will send an error to the host bitcoin app and return false.
 *
 * @param query Reference to an instance of btc_query_t containing query
 * received from host app
 * @param which_request The expected request type enum
 *
 * @return bool Indicating if the check succeeded or failed
 * @retval true If the query contains the expected request
 * @retval false If the query does not contain the expected request
 */
static bool check_which_request(const btc_query_t *query,
                                pb_size_t which_request);

/**
 * @brief Validates the derivation paths received in the request from host
 * @details The function validates each path index in the request. If any
 * invalid index is detected, the function will send an error to the host and
 * return false.
 *
 * @param request Reference to an instance of btc_get_public_key_request_t
 * @return bool Indicating if the verification passed or failed
 * @retval true If the derivation path entries are valid
 * @retval false If any of the derivation path entries are invalid
 */
static bool validate_request_data(btc_get_public_key_request_t *request);

/**
 * @brief Derives uncompressed public key and address from the provided seed
 * @details The function can provided both public_key and address. It accepts
 * NULL in output parameters and handles accordingly. The function
 * also manages all the terminal errors during derivation/encoding, in which
 * case it will return 0 and send a relevant error to the host closing the
 * request-response pair. All the errors/invalid cases are conveyed to the host
 * as unknown_error = 1 because we expect the data validation was success.
 *
 * @param seed Reference to the wallet seed generated from X1 Card
 * @param path Derivation path of the node to be derived
 * @param path_length Expected length of the provided derivation path
 * @param public_key Storage location for raw uncompressed public key
 * @param address Storage location for encoded public address
 *
 * @return size_t length of the derived public address
 * @retval 0 If derivation failed
 */
static size_t btc_get_address(const uint8_t *seed,
                              const uint32_t *path,
                              uint32_t path_length,
                              uint8_t *public_key,
                              char *address);

/**
 * @brief Takes and sends the uncompressed public key to the host as
 * btc_result_t
 *
 * @param public_key An immutable reference to uncompressed public key buffer
 */
static void send_public_key(const uint8_t *public_key);

/*****************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/

static bool check_which_request(const btc_query_t *query,
                                pb_size_t which_request) {
  if (which_request != query->get_public_key.which_request) {
    btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_REQUEST);
    return false;
  }

  return true;
}

static bool validate_request_data(btc_get_public_key_request_t *request) {
  bool status = true;

  if (!btc_derivation_path_guard(request->initiate.derivation_path,
                                 request->initiate.derivation_path_count)) {
    btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_DATA);
    status = false;
  }
  return status;
}

static size_t btc_get_address(const uint8_t *seed,
                              const uint32_t *path,
                              uint32_t path_length,
                              uint8_t *public_key,
                              char *address) {
  HDNode node = {0};
  char addr[50] = "";
  size_t address_length = 0;

  if (!derive_hdnode_from_path(
          path, path_length, SECP256K1_NAME, seed, &node)) {
    // send unknown error; unknown failure reason
    btc_send_error(ERROR_COMMON_ERROR_UNKNOWN_ERROR_TAG, 1);
    memzero(&node, sizeof(HDNode));
    return address_length;
  }

  switch (path[0]) {
    case NATIVE_SEGWIT:
      // ignoring the return status and handling by size of address
      btc_get_segwit_addr(node.public_key,
                          sizeof(node.public_key),
                          g_btc_app->bech32_hrp,
                          addr);
      break;
    case NON_SEGWIT:
      hdnode_get_address(&node, g_btc_app->p2pkh_addr_ver, addr, 35);
      break;
    case PURPOSE_SEGWIT:
      ecdsa_get_address_segwit_p2sh(node.public_key,
                                    g_btc_app->p2sh_addr_ver,
                                    node.curve->hasher_pubkey,
                                    node.curve->hasher_base58,
                                    addr,
                                    36);
      break;

    // TODO: add support for taproot
    default:
      break;
  }

  address_length = strnlen(addr, sizeof(addr));
  if (0 >= address_length) {
    // address encoding failed
    btc_send_error(ERROR_COMMON_ERROR_UNKNOWN_ERROR_TAG, 1);
  }
  if (NULL != public_key) {
    ecdsa_uncompress_pubkey(
        get_curve_by_name(SECP256K1_NAME)->params, node.public_key, public_key);
  }
  if (NULL != address) {
    memcpy(address, addr, address_length);
  }
  memzero(&node, sizeof(HDNode));
  return address_length;
}

static void send_public_key(const uint8_t *public_key) {
  btc_result_t response = init_btc_result(BTC_RESULT_GET_PUBLIC_KEY_TAG);
  response.get_public_key.which_response =
      BTC_GET_PUBLIC_KEY_RESPONSE_RESULT_TAG;
  memcpy(response.get_public_key.result.public_key, public_key, 65);
  btc_send_result(&response);
}

/*****************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTIONS
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTIONS
 *****************************************************************************/

void btc_get_pub_key(btc_query_t *query) {
  char wallet_name[NAME_SIZE] = "";
  char msg[100] = "";
  uint8_t seed[64] = {0};
  uint8_t public_key[65] = {0};
  btc_get_public_key_intiate_request_t *init_req =
      &query->get_public_key.initiate;

  if (!check_which_request(query, BTC_GET_PUBLIC_KEY_REQUEST_INITIATE_TAG) ||
      !validate_request_data(&query->get_public_key) ||
      !get_wallet_name_by_id(query->get_xpubs.initiate.wallet_id,
                             (uint8_t *)wallet_name,
                             btc_send_error)) {
    return;
  }

  snprintf(
      msg, sizeof(msg), UI_TEXT_RECEIVE_PROMPT, g_btc_app->name, wallet_name);
  // Take user consent to export public key for the wallet
  if (!core_confirmation(msg, btc_send_error)) {
    return;
  }

  set_app_flow_status(BTC_GET_PUBLIC_KEY_STATUS_CONFIRM);

  if (!reconstruct_seed(
          query->get_xpubs.initiate.wallet_id, &seed[0], btc_send_error)) {
    memzero(seed, sizeof(seed));
    return;
  }

  set_app_flow_status(BTC_GET_PUBLIC_KEY_STATUS_SEED_GENERATED);
  const uint32_t *path = init_req->derivation_path;
  uint32_t path_length = init_req->derivation_path_count;

  delay_scr_init(ui_text_processing, DELAY_SHORT);
  size_t length = btc_get_address(seed, path, path_length, public_key, msg);
  memzero(seed, sizeof(seed));
  if (0 < length &&
      true == core_scroll_page(ui_text_receive_on, msg, btc_send_error)) {
    set_app_flow_status(BTC_GET_PUBLIC_KEY_STATUS_VERIFY);
    send_public_key(public_key);
    delay_scr_init(ui_text_check_cysync_app, DELAY_TIME);
  }
}
