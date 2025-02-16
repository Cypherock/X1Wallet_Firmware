/**
 * @file    icp_txn.c
 * @author  Cypherock X1 Team
 * @brief   Source file to handle transaction signing logic for ICP protocol
 *
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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "base58.h"
#include "icp/sign_txn.pb.h"
#include "icp_api.h"
#include "icp_context.h"
#include "icp_helpers.h"
#include "icp_priv.h"
#include "icp_txn_helpers.h"
#include "reconstruct_wallet_flow.h"
#include "sha2.h"
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
typedef icp_sign_txn_signature_response_t sig_t;

/*****************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * @brief Checks if the provided query contains expected request.
 * @details The function performs the check on the request type and if the check
 * fails, then it will send an error to the host bitcoin app and return false.
 *
 * @param query Reference to an instance of icp_query_t containing query
 * received from host app
 * @param which_request The expected request type enum
 *
 * @return bool Indicating if the check succeeded or failed
 * @retval true If the query contains the expected request
 * @retval false If the query does not contain the expected request
 */
static bool check_which_request(const icp_query_t *query,
                                pb_size_t which_request);

/**
 * @brief The function prepares and sends empty responses
 *
 * @param which_response Constant value for the response type to be sent
 */
static void send_response(const pb_size_t which_response);

/**
 * @brief Validates the derivation path received in the request from host
 * @details The function validates the provided account derivation path in the
 * request. If invalid path is detected, the function will send an error to the
 * host and return false.
 *
 * @param request Reference to an instance of icp_sign_txn_request_t
 * @return bool Indicating if the verification passed or failed
 * @retval true If all the derivation path entries are valid
 * @retval false If any of the derivation path entries are invalid
 */
static bool validate_request_data(const icp_sign_txn_request_t *request);

/**
 * @brief Takes already received and decoded query for the user confirmation.
 * @details The function will verify if the query contains the
 * ICP_SIGN_TXN_REQUEST_INITIATE_TAG type of request. Additionally, the
 * wallet-id is validated for sanity and the derivation path for the account is
 * also validated. After the validations, user is prompted about the action for
 * confirmation. The function returns true indicating all the validation and
 * user confirmation was a success. The function also duplicates the data from
 * query into the icp_txn_context  for further processing.
 *
 * @param query Constant reference to the decoded query received from the host
 *
 * @return bool Indicating if the function actions succeeded or failed
 * @retval true If all the validation and user confirmation was positive
 * @retval false If any of the validation or user confirmation was negative
 */
static bool handle_initiate_query(const icp_query_t *query);

/**
 * @brief Receives unsigned txn from the host. If reception is successful, it
 * also parses the txn to ensure it's validity.
 * @note In case of any failure, a corresponding message is conveyed to the host
 *
 * @param query Reference to buffer of type icp_query_t
 * @return true If the txn is received in the internal buffers and is valid
 * @return false If the txn could not be received or it's validation failed
 */
static bool fetch_valid_input(icp_query_t *query);

/**
 * @brief This function executes user verification flow of the unsigned txn
 * received from the host.
 * @details The user verification flow is different for different type of action
 * types identified from the unsigned txn
 * @note This function expected that the unsigned txn is parsed using the helper
 * function as only few action types are supported currently.
 *
 * @return true If the user accepted the transaction display
 * @return false If any user rejection occured or P0 event occured
 */
static bool get_user_verification(void);

/**
 * @brief Calculates ED25519 curve based signature over the digest of the user
 * verified unsigned txn.
 * @details Seed reconstruction takes place within this function
 *
 * @param signature_buffer Reference to buffer where the signature will be
 * populated
 * @return true If the signature was computed successfully
 * @return false If signature could not be computed - maybe due to some error
 * during seed reconstruction phase
 */
static bool sign_txn(sig_t *signature);

/**
 * @brief Sends signature of the ICP unsigned txn to the host
 * @details The function waits for the host to send a request of type
 * ICP_SIGN_TXN_REQUEST_SIGNATURE_TAG and sends the response
 *
 * @param query Reference to buffer of type icp_query_t
 * @param signature Reference to signature to be sent to the host
 * @return true If the signature was sent successfully
 * @return false If the signature could not be sent - maybe due to and P0 event
 * or invalid request received from the host
 */
static bool send_signature(icp_query_t *query, const sig_t *signature);

/*****************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/
static icp_txn_context_t *icp_txn_context = NULL;

/*****************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTIONS
 *****************************************************************************/
static bool check_which_request(const icp_query_t *query,
                                pb_size_t which_request) {
  if (which_request != query->sign_txn.which_request) {
    icp_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_REQUEST);
    return false;
  }

  return true;
}

static void send_response(const pb_size_t which_response) {
  icp_result_t result = init_icp_result(ICP_RESULT_SIGN_TXN_TAG);
  result.sign_txn.which_response = which_response;
  icp_send_result(&result);
}

static bool validate_request_data(const icp_sign_txn_request_t *request) {
  bool status = true;

  if (!icp_derivation_path_guard(request->initiate.derivation_path,
                                 request->initiate.derivation_path_count)) {
    icp_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_DATA);
    status = false;
  }
  return status;
}

static bool handle_initiate_query(const icp_query_t *query) {
  char wallet_name[NAME_SIZE] = "";
  char msg[100] = "";

  // TODO: Handle wallet search failures - eg: Wallet ID not found, Wallet
  // ID found but is invalid/locked wallet
  if (!check_which_request(query, ICP_SIGN_TXN_REQUEST_INITIATE_TAG) ||
      !validate_request_data(&query->sign_txn) ||
      !get_wallet_name_by_id(query->sign_txn.initiate.wallet_id,
                             (uint8_t *)wallet_name,
                             icp_send_error)) {
    return false;
  }

  snprintf(msg, sizeof(msg), UI_TEXT_SIGN_TXN_PROMPT, ICP_NAME, wallet_name);
  // Take user consent to sign transaction for the wallet
  if (!core_confirmation(msg, icp_send_error)) {
    return false;
  }

  set_app_flow_status(ICP_SIGN_TXN_STATUS_CONFIRM);
  memcpy(&icp_txn_context->init_info,
         &query->sign_txn.initiate,
         sizeof(icp_sign_txn_initiate_request_t));

  send_response(ICP_SIGN_TXN_RESPONSE_CONFIRMATION_TAG);
  // show processing screen for a minimum duration (additional time will add due
  // to actual processing)
  delay_scr_init(ui_text_processing, DELAY_SHORT);
  return true;
}

static bool fetch_valid_input(icp_query_t *query) {
  if (!icp_get_query(query, ICP_QUERY_SIGN_TXN_TAG) &&
      !check_which_request(query, ICP_SIGN_TXN_REQUEST_TXN_DATA_TAG)) {
    return false;
  }

  const icp_sign_txn_data_t *txn_data = &query->sign_txn.txn_data;
  if (txn_data->has_icp_transfer_req == false) {
    return false;
  }

  icp_txn_context->icp_transfer_req = &txn_data->icp_transfer_req;

  // Verify request_type = "call"
  if (memcmp(icp_txn_context->icp_transfer_req->request_type, "call", 4) != 0) {
    return false;
  }

  // Verify method_name = "transfer"
  if (memcmp(icp_txn_context->icp_transfer_req->method_name, "transfer", 8) !=
      0) {
    return false;
  }

  send_response(ICP_SIGN_TXN_RESPONSE_DATA_ACCEPTED_TAG);

  icp_txn_context->raw_icp_transfer_txn =
      (icp_transfer_t *)malloc(sizeof(icp_transfer_t));

  if (!icp_parse_transfer_txn(icp_txn_context->icp_transfer_req->arg.bytes,
                              icp_txn_context->icp_transfer_req->arg.size,
                              icp_txn_context->raw_icp_transfer_txn)) {
    icp_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_DATA);
    return false;
  }

  return true;
}

static bool get_user_verification(void) {
  const icp_transfer_t *decoded_utxn = icp_txn_context->raw_icp_transfer_txn;

  char to_address[ICP_ACCOUNT_ID_LENGTH * 2 + 1] = "";

  byte_array_to_hex_string(decoded_utxn->to,
                           ICP_ACCOUNT_ID_LENGTH,
                           to_address,
                           ICP_ACCOUNT_ID_LENGTH * 2 + 1);

  if (!core_scroll_page(ui_text_verify_address, to_address, icp_send_error)) {
    return false;
  }

  // verify recipient amount
  uint64_t amount = 0;
  memcpy(&amount, &decoded_utxn->amount->e8s, sizeof(uint64_t));
  char amount_string[30] = {'\0'};
  double decimal_amount = (double)amount;
  decimal_amount *= 1e-8;
  snprintf(amount_string, sizeof(amount_string), "%.8f", decimal_amount);

  char display[100] = {'\0'};
  snprintf(display,
           sizeof(display),
           UI_TEXT_VERIFY_AMOUNT,
           amount_string,
           ICP_LUNIT);

  if (!core_confirmation(display, icp_send_error)) {
    return false;
  }

  // verify transaction fee
  uint64_t fee = 0;
  memcpy(&fee, &decoded_utxn->fee->e8s, sizeof(uint64_t));
  char fee_string[30] = {'\0'};
  double decimal_fee = (double)fee;
  decimal_fee *= 1e-8;
  snprintf(fee_string, sizeof(fee_string), "%.8f", decimal_fee);

  snprintf(
      display, sizeof(display), UI_TEXT_SEND_TXN_FEE, fee_string, ICP_LUNIT);
  if (!core_scroll_page(UI_TEXT_TXN_FEE, display, icp_send_error)) {
    return false;
  }

  // verify memo
  char display_memo[50] = {'\0'};
  snprintf(display_memo,
           sizeof(display_memo),
           UI_TEXT_VERIFY_MEMO,
           decoded_utxn->memo);

  if (!core_confirmation(display_memo, icp_send_error)) {
    return false;
  }

  set_app_flow_status(ICP_SIGN_TXN_STATUS_VERIFY);

  return true;
}

static bool sign_txn(sig_t *signature) {
  uint8_t seed[64] = {0};
  if (!reconstruct_seed(
          icp_txn_context->init_info.wallet_id, seed, icp_send_error)) {
    memzero(seed, sizeof(seed));
    // TODO: handle errors of reconstruction flow
    return false;
  }

  set_app_flow_status(ICP_SIGN_TXN_STATUS_SEED_GENERATED);

  uint8_t request_id[32];
  hash_icp_transfer_request(icp_txn_context->icp_transfer_req, request_id);

  // "\x0Aic-request"
  uint8_t domain_separator[11] = {
      10, 105, 99, 45, 114, 101, 113, 117, 101, 115, 116};

  uint8_t result[43] = {0};
  memcpy(result, domain_separator, 11);
  memcpy(result + 11, request_id, 32);

  uint8_t digest[32] = {0};
  sha256_Raw(result, 43, digest);

  HDNode hdnode = {0};
  derive_hdnode_from_path(icp_txn_context->init_info.derivation_path,
                          icp_txn_context->init_info.derivation_path_count,
                          SECP256K1_NAME,
                          seed,
                          &hdnode);

  ecdsa_sign_digest(
      &secp256k1, hdnode.private_key, digest, signature->signature, NULL, NULL);

  memzero(digest, sizeof(digest));
  memzero(seed, sizeof(seed));
  memzero(&hdnode, sizeof(hdnode));

  return true;
}

static bool send_signature(icp_query_t *query, const sig_t *signature) {
  icp_result_t result = init_icp_result(ICP_RESULT_SIGN_TXN_TAG);
  result.sign_txn.which_response = ICP_SIGN_TXN_RESPONSE_SIGNATURE_TAG;

  if (!icp_get_query(query, ICP_QUERY_SIGN_TXN_TAG) ||
      !check_which_request(query, ICP_SIGN_TXN_REQUEST_SIGNATURE_TAG)) {
    return false;
  }

  memcpy(&result.sign_txn.signature, signature, sizeof(sig_t));

  icp_send_result(&result);
  return true;
}

/*****************************************************************************
 * GLOBAL FUNCTIONS
 *****************************************************************************/

void icp_sign_transaction(icp_query_t *query) {
  icp_txn_context = (icp_txn_context_t *)malloc(sizeof(icp_txn_context_t));
  memzero(icp_txn_context, sizeof(icp_txn_context_t));

  sig_t signature = {0};

  if (handle_initiate_query(query) && fetch_valid_input(query) &&
      get_user_verification() && sign_txn(&signature) &&
      send_signature(query, &signature)) {
    delay_scr_init(ui_text_check_cysync, DELAY_TIME);
  }

  if (icp_txn_context) {
    free(icp_txn_context);
    icp_txn_context = NULL;
  }

  return;
}
