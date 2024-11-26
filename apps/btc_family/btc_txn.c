/**
 * @file    btc_txn.c
 * @author  Cypherock X1 Team
 * @brief   Bitcoin family transaction flow.
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
#include "btc_script.h"
#include "btc_txn_helpers.h"
#include "coin_utils.h"
#include "constant_texts.h"
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

#define TXN_MAX_INPUTS 200
#define TXN_MAX_OUTPUTS 200
#define TXN_MAX_UTXO_SUM ((TXN_MAX_INPUTS + TXN_MAX_OUTPUTS) / 2)
#define SCRIPT_SIG_SIZE 128

/*****************************************************************************
 * PRIVATE TYPEDEFS
 *****************************************************************************/

typedef btc_sign_txn_signature_response_signature_t scrip_sig_t;

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
 * @brief Validates the derivation path received in the request from host
 * @details The function validates the provided account derivation path in the
 * request. If invalid path is detected, the function will send an error to the
 * host and return false.
 *
 * @param request Reference to an instance of btc_sign_txn_request_t
 * @return bool Indicating if the verification passed or failed
 * @retval true If all the derivation path entries are valid
 * @retval false If any of the derivation path entries are invalid
 */
static bool validate_request_data(const btc_sign_txn_request_t *request);

/**
 * @brief The function prepares and sends empty responses
 *
 * @param which_response Constant value for the response type to be sent
 */
static void send_response(pb_size_t which_response);

/**
 * @brief Takes already received and decoded query for the user confirmation.
 * @details The function will verify if the query contains the BTC_SIGN_TXN type
 * of request. Additionally, the wallet-id is validated for sanity and the
 * derivation path for the account is also validated. After the validations,
 * user is prompted about the action for confirmation. The function returns true
 * indicating all the validation and user confirmation was a success. The
 * function also duplicates the data from query into the btc_txn_context  for
 * further processing.
 *
 * @param query Constant reference to the decoded query received from the host
 *
 * @return bool Indicating if the function actions succeeded or failed
 * @retval true If all the validation and user confirmation was positive
 * @retval false If any of the validation or user confirmation was negative
 */
static bool handle_initiate_query(const btc_query_t *query);

/**
 * @brief Handles fetching of the metadata/top-level transaction elements
 * @details The function waits on USB event then decoding and validation of the
 * received query. Post validation, based on the values in the query, the
 * function allocates memory for storing inputs & outputs in btc_txn_context.
 * Also, the data received in the query is duplicated into btc_txn_context.
 * Refer transaction format here: https://en.bitcoin.it/wiki/Transaction or
 * https://developer.bitcoin.org/devguide/transactions.html or
 * https://developer.bitcoin.org/reference/transactions.html#raw-transaction-format
 *
 * @param query Reference to storage for decoding query from host
 *
 * @return bool Indicating if the function actions succeeded or failed
 */
static bool fetch_transaction_meta(btc_query_t *query);

/**
 * @brief Fetches each input and along with its corresponding raw transaction
 * for verification
 * @details The function will try to fetch and consequently verify each input
 * by referring to the declared input count in btc_txn_context . The function
 * will duplicate each input transaction information into btc_txn_context.
 *
 * @param query Reference to an instance of btc_query_t for storing the
 * transient inputs.
 *
 * @return bool Indicating if all the inputs are received and verified
 * @retval true If all the inputs are fetched and verified
 * @retval flase If any of the inputs failed verification or weren't fetched
 */
static bool fetch_valid_input(btc_query_t *query);

/**
 * @brief Fetches the outputs list for the transaction
 * @details The function refers to the number of outputs declared in the
 * btc_txn_context . It will also duplicate the received output.
 *
 * @param query Reference to an instance of btc_query_t for storing the
 * transient outputs.
 *
 * @return bool Indicating if all the outputs were fetched
 * @retval true If all the outputs were fetched
 */
static bool fetch_valid_output(btc_query_t *query);

/**
 * @brief Aggregates user consent for all outputs and the transaction fee
 * @details The function encodes all the receiver addresses along with their
 * corresponding transfer value in BTC. It also calculates the transaction fee
 * and checks for exaggerated fees. The user is assisted with additional
 * prompt/warning if the function detects the high fee (for calculation of the
 * upper limit of fee see get_transaction_fee_threshold(). The exact fee amount
 * is also confirmed with the user.
 *
 * @return bool Indicating if the user confirmed the transaction
 * @retval true If user confirmed the fee (along with high fee prompt if
 * applicable) and all the receiver addresses along with the corresponding
 * value.
 * @retval false Immediate return if any of the confirmation is disapproved
 */
static bool get_user_verification();

/**
 * @brief Validates the change output for an exact match with wallet's derived
 * change address.
 * @details The function ensures that the change output is spendable by the
 * wallet by verifying the raw public address in the output script. The function
 * ensures all its local HDNode instances are cleared before exit to ensure
 * safety from key leakage.
 *
 * @param acc_node Reference to the valid account node
 * @return bool Indicating if the validation passed.
 * @retval true If the validation passed.
 * @retval false If the validation failed.
 */
static bool validate_change_address(const HDNode *acc_node);

/**
 * @brief Signs all the inputs following SIGHASH_ALL type and prepares script
 * sigs.
 * @details The function internally calls wallet reconstruction sub-flow to get
 * access to the seed. Each input is signed with their respective private key
 * derived at the specified path. The function additionally validates the change
 * output address for sanity. The function will ensure clearing of all the keys
 * and other sensitive data (such as seed, HDNode, etc.) before exiting and
 * should ensure no leakage of secret data.
 *
 * @param signatures Reference to the buffer sufficient enough for storing all
 * the scriptSigs of the transaction being signed. Should be able to store
 * >=btc_txn_context->metadata.input_count.
 *
 * @return bool Indicating if scriptSig for all the provided inputs was
 * successfully generated
 * @retval true If all the scriptSigs are generated without any error
 * @retval false If any of the scriptSig failed to generate
 */
static bool sign_input(scrip_sig_t *signatures);

/**
 * @brief Sends the generated scriptSigs to the host one-at-a-time
 *
 * @param query Reference to an instance of btc_query_t to store transient
 * requests from the host
 * @param sigs Reference to a list of prepared scriptSigs. The buffer should
 * hold at least btc_txn_context->metadata.input_count of scriptSigs
 *
 * @return bool Indicating if all the scriptSig is sent to the host
 * @retval true If all the scriptSig was sent to host successfully
 * @retval false If the host responded with unknown/wrong query
 */
static bool send_script_sig(btc_query_t *query, const scrip_sig_t *sigs);

/*****************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/

static btc_txn_context_t *btc_txn_context = NULL;

/*****************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTIONS
 *****************************************************************************/

static bool check_which_request(const btc_query_t *query,
                                pb_size_t which_request) {
  if (which_request != query->sign_txn.which_request) {
    btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_REQUEST);
    return false;
  }

  return true;
}

static bool validate_request_data(const btc_sign_txn_request_t *request) {
  bool status = true;

  if (!btc_derivation_path_guard(request->initiate.derivation_path,
                                 request->initiate.derivation_path_count)) {
    btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_DATA);
    status = false;
  }
  return status;
}

static void send_response(const pb_size_t which_response) {
  btc_result_t result = init_btc_result(BTC_RESULT_SIGN_TXN_TAG);
  result.sign_txn.which_response = which_response;
  btc_send_result(&result);
}

static bool handle_initiate_query(const btc_query_t *query) {
  char wallet_name[NAME_SIZE] = "";
  char msg[100] = "";

  if (!check_which_request(query, BTC_SIGN_TXN_REQUEST_INITIATE_TAG) ||
      !validate_request_data(&query->sign_txn) ||
      !get_wallet_name_by_id(query->sign_txn.initiate.wallet_id,
                             (uint8_t *)wallet_name,
                             btc_send_error)) {
    return false;
  }

  snprintf(
      msg, sizeof(msg), UI_TEXT_BTC_SEND_PROMPT, g_btc_app->name, wallet_name);
  // Take user consent to sign transaction for the wallet
  if (!core_confirmation(msg, btc_send_error)) {
    return false;
  }

  set_app_flow_status(BTC_SIGN_TXN_STATUS_CONFIRM);
  memcpy(&btc_txn_context->init_info,
         &query->sign_txn.initiate,
         sizeof(btc_sign_txn_initiate_request_t));
  send_response(BTC_SIGN_TXN_RESPONSE_CONFIRMATION_TAG);
  // show processing screen for a minimum duration (additional time will add due
  // to actual processing)
  delay_scr_init(ui_text_processing, DELAY_SHORT);
  return true;
}

static bool fetch_transaction_meta(btc_query_t *query) {
  if (!btc_get_query(query, BTC_QUERY_SIGN_TXN_TAG) ||
      !check_which_request(query, BTC_SIGN_TXN_REQUEST_META_TAG)) {
    return false;
  }
  uint32_t in_count = query->sign_txn.meta.input_count;
  uint32_t out_count = query->sign_txn.meta.output_count;
  // check important information for supported/compatibility
  if (0x00000001 != query->sign_txn.meta.sighash || 0 == in_count ||
      0 == out_count || TXN_MAX_INPUTS < in_count ||
      TXN_MAX_OUTPUTS < out_count ||
      TXN_MAX_UTXO_SUM < (in_count + out_count)) {
    /** Do not accept transaction with empty input/output.
     * Only accept SIGHASH_ALL for sighash type More info:
     * https://wiki.bitcoinsv.io/index.php/SIGHASH_flags
     */
    btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_DATA);
    return false;
  }

  // we now know the number of input and outputs
  // allocate memory for input and outputs in btc_txn_context
  memcpy(&btc_txn_context->metadata,
         &query->sign_txn.meta,
         sizeof(btc_sign_txn_metadata_t));
  btc_txn_context->inputs = (btc_txn_input_t *)malloc(
      sizeof(btc_txn_input_t) * btc_txn_context->metadata.input_count);
  btc_txn_context->outputs = (btc_sign_txn_output_t *)malloc(
      sizeof(btc_sign_txn_output_t) * btc_txn_context->metadata.output_count);
  // TODO: check if malloc failed; report to host and exit
  send_response(BTC_SIGN_TXN_RESPONSE_META_ACCEPTED_TAG);
  return true;
}

static bool fetch_valid_input(btc_query_t *query) {
  // Validate inputs for safety from attack. Ref:
  // https://blog.trezor.io/details-of-firmware-updates-for-trezor-one-version-1-9-1-and-trezor-model-t-version-2-3-1-1eba8f60f2dd
  for (int idx = 0; idx < btc_txn_context->metadata.input_count; idx++) {
    if (!btc_get_query(query, BTC_QUERY_SIGN_TXN_TAG) ||
        !check_which_request(query, BTC_SIGN_TXN_REQUEST_INPUT_TAG)) {
      return false;
    }
    // copy prevtxn details for later verification
    btc_sign_txn_input_t *txin = malloc(sizeof(btc_sign_txn_input_t));
    memcpy(txin, &query->sign_txn.input, sizeof(btc_sign_txn_input_t));

    // P2PK 68, P2PKH 25 (21 excluding OP_CODES), P2WPKH 22, P2MS ~, P2SH 23 (21
    // excluding OP_CODES). refer https://learnmeabitcoin.com/technical/script
    // for explanation. Currently, the device can spend P2PKH or P2WPKH inputs
    const btc_script_type_e type = btc_get_script_type(
        txin->script_pub_key.bytes, txin->script_pub_key.size);

    // clone the input details into btc_txn_context
    btc_txn_input_t *input = &btc_txn_context->inputs[idx];
    input->prev_output_index = txin->prev_output_index;
    input->address_index = txin->address_index;
    input->change_index = txin->change_index;
    input->value = txin->value;
    input->sequence = txin->sequence;
    input->script_pub_key.size = txin->script_pub_key.size;
    memcpy(input->prev_txn_hash, txin->prev_txn_hash, 32);
    memcpy(input->script_pub_key.bytes,
           txin->script_pub_key.bytes,
           input->script_pub_key.size);

    // send accepted response to indicate validation of input passed
    send_response(BTC_SIGN_TXN_RESPONSE_INPUT_ACCEPTED_TAG);

    // TODO: ensure only valid input for the path are being provided. spending a
    // segwit input on the legacy derivation path does not make sense.
    // verify transaction details and discard the raw-transaction (prev_txn)
    btc_result_t response = init_btc_result(BTC_RESULT_SIGN_TXN_TAG);
    const btc_prev_txn_chunk_t *prev_txn = &(query->sign_txn.prev_txn_chunk);
    const common_chunk_payload_t *payload = &(prev_txn->chunk_payload);
    const common_chunk_payload_chunk_t *chunk = &(payload->chunk);

    uint32_t total_size = 0;
    uint32_t size = 0;
    int status = 4;

    btc_verify_input_t verify_input_data;
    memzero(&(verify_input_data), sizeof(btc_verify_input_t));

    while (1) {
      // req prev txn chunk from host
      if (!btc_get_query(query, BTC_QUERY_SIGN_TXN_TAG) ||
          !check_which_request(query,
                               BTC_SIGN_TXN_REQUEST_PREV_TXN_CHUNK_TAG)) {
        return false;
      }
      // init details from first chunk
      if (0 == size) {
        verify_input_data.chunk_total = payload->total_chunks;
        total_size = chunk->size + payload->remaining_size;

        verify_input_data.size_last_chunk = total_size % CHUNK_SIZE;
        if (verify_input_data.size_last_chunk < 4) {
          verify_input_data.is_locktime_split = true;
        }
      }

      if (false == query->sign_txn.prev_txn_chunk.has_chunk_payload ||
          payload->chunk_index >= payload->total_chunks ||
          size + chunk->size > total_size) {
        btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                       ERROR_DATA_FLOW_INVALID_DATA);
        return false;
      }
      status = btc_verify_input(
          chunk->bytes, payload->chunk_index, &verify_input_data, txin);
      size += chunk->size;

      // Send chunk ack to host
      response.sign_txn.which_response =
          BTC_SIGN_TXN_RESPONSE_PREV_TXN_CHUNK_ACCEPTED_TAG;
      response.sign_txn.prev_txn_chunk_accepted.has_chunk_ack = true;
      response.sign_txn.prev_txn_chunk_accepted.chunk_ack.chunk_index =
          payload->chunk_index;
      btc_send_result(&response);

      // If no data remaining to be received from the host, then exit
      if (0 == payload->remaining_size ||
          payload->chunk_index + 1 == payload->total_chunks) {
        break;
      }

      if (4 != status) {
        btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                       ERROR_DATA_FLOW_INVALID_DATA);
        break;
      }
    }
    // Free txin after use
    free(txin);
    if (total_size != size) {
      btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                     ERROR_DATA_FLOW_INVALID_DATA);
      return false;
    }

    if ((SCRIPT_TYPE_P2PKH != type && SCRIPT_TYPE_P2WPKH != type) ||
        0 != status) {
      // input validation failed, terminate immediately
      btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                     ERROR_DATA_FLOW_INVALID_DATA);
      return false;
    }
  }
  return true;
}

static bool fetch_valid_output(btc_query_t *query) {
  // track if it is a zero valued transaction; all input is going into fee
  bool zero_value_transaction = true;
  // restrict multiple change outputs; it is wastage of fee for user; this is
  // a loose check as it is externally controlled
  bool change_detected = false;
  btc_txn_context->change_output_idx = -1;

  for (int idx = 0; idx < btc_txn_context->metadata.output_count; idx++) {
    if (!btc_get_query(query, BTC_QUERY_SIGN_TXN_TAG) ||
        !check_which_request(query, BTC_SIGN_TXN_REQUEST_OUTPUT_TAG)) {
      return false;
    }

    btc_sign_txn_output_t *output = &query->sign_txn.output;
    memcpy(
        &btc_txn_context->outputs[idx], output, sizeof(btc_sign_txn_output_t));
    if (0 != output->value) {
      zero_value_transaction = false;
    }
    const btc_script_type_e type = btc_get_script_type(
        output->script_pub_key.bytes, output->script_pub_key.size);
    if (SCRIPT_TYPE_P2MS == type || SCRIPT_TYPE_P2PK == type ||
        SCRIPT_TYPE_NONSTANDARD == type ||
        (SCRIPT_TYPE_NULL_DATA == type && 0 != output->value) ||
        (true == change_detected && true == output->is_change)) {
      // ensure output type is standard & we support verification by user
      // ensure any funds are not being locked (not spendable) to NULL_DATA
      // ensure exactly one change address is present/declared
      btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                     ERROR_DATA_FLOW_INVALID_DATA);
      return false;
    }

    if (output->is_change) {
      // first change output declaration detected; store for quick access
      change_detected = true;
      btc_txn_context->change_output_idx = idx;
    }
    // send accepted response to indicate validation of output passed
    send_response(BTC_SIGN_TXN_RESPONSE_OUTPUT_ACCEPTED_TAG);
  }
  if (true == zero_value_transaction) {
    // do not allow zero valued transaction; all input is going into fee
    btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_DATA);
    return false;
  }
  return true;
}

static bool get_user_verification() {
  char title[20] = "";
  char value[100] = "";
  char address[100] = "";

  for (int idx = 0; idx < btc_txn_context->metadata.output_count; idx++) {
    btc_sign_txn_output_t *output = &btc_txn_context->outputs[idx];
    btc_sign_txn_output_script_pub_key_t *script = &output->script_pub_key;
    snprintf(title, sizeof(title), UI_TEXT_BTC_RECEIVER, (idx + 1));

    if (true == output->is_change) {
      // do not show the change outputs to user
      continue;
    }
    format_value(output->value, value, sizeof(value));
    int status = btc_get_script_pub_address(
        script->bytes, script->size, address, sizeof(address));
    if (1 > status) {
      // send error status as value for unknown error
      btc_send_error(ERROR_COMMON_ERROR_UNKNOWN_ERROR_TAG, status);
      return false;
    }
    if (!core_scroll_page(title, address, btc_send_error) ||
        !core_scroll_page(title, value, btc_send_error)) {
      return false;
    }
  }

  // calculate fee in various forms
  uint64_t max_fee = get_transaction_fee_threshold(btc_txn_context);
  uint64_t fee_in_satoshi = 0;

  if (!btc_get_txn_fee(btc_txn_context, &fee_in_satoshi)) {
    // The transaction is overspending
    btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_DATA);
    return false;
  }

  // all the receivers are verified, check fee limit & show the fee
  // validate fee limit is not too high and acceptable to user
  if (fee_in_satoshi > max_fee &&
      !core_confirmation(ui_text_warning_txn_fee_too_high, btc_send_error)) {
    return false;
  }

  format_value(fee_in_satoshi, value, sizeof(value));
  if (!core_scroll_page(UI_TEXT_BTC_FEE, value, btc_send_error)) {
    return false;
  }
  set_app_flow_status(BTC_SIGN_TXN_STATUS_VERIFY);
  return true;
}

static bool validate_change_address(const HDNode *acc_node) {
  bool status = false;
  if (btc_txn_context->change_output_idx == -1) {
    // txn w/o change output should go through
    return true;
  }

  HDNode t_node = {0};
  int idx = btc_txn_context->change_output_idx;
  const btc_sign_txn_output_t *output = &btc_txn_context->outputs[idx];
  if (false == output->has_changes_index || false == output->is_change) {
    return status;
  }

  memcpy(&t_node, acc_node, sizeof(HDNode));
  hdnode_private_ckd(&t_node, 1);
  hdnode_private_ckd(&t_node, output->changes_index);
  hdnode_fill_public_key(&t_node);
  status = btc_check_script_address(output->script_pub_key.bytes,
                                    output->script_pub_key.size,
                                    t_node.public_key);
  memzero(&t_node, sizeof(HDNode));
  return status;
}

static bool sign_input(scrip_sig_t *signatures) {
  uint8_t buffer[64] = {0};
  HDNode node = {0};
  HDNode t_node = {0};
  bool status = false;
  const uint32_t *hd_path = btc_txn_context->init_info.derivation_path;
  const ecdsa_curve *curve = get_curve_by_name(SECP256K1_NAME)->params;
  if (!reconstruct_seed(
          btc_txn_context->init_info.wallet_id, buffer, btc_send_error)) {
    memzero(buffer, sizeof(buffer));
    return status;
  }

  set_app_flow_status(BTC_SIGN_TXN_STATUS_SEED_GENERATED);

  // populate hashes cache for segwit transaction types
  btc_segwit_init_cache(btc_txn_context);
  if (!derive_hdnode_from_path(hd_path, 3, SECP256K1_NAME, buffer, &node) ||
      false == validate_change_address(&node)) {
    btc_send_error(ERROR_COMMON_ERROR_CORRUPT_DATA_TAG,
                   ERROR_DATA_FLOW_INVALID_DATA);
    memzero(&node, sizeof(HDNode));
    memzero(buffer, sizeof(buffer));
    return status;
  }

  status = true;
  for (int idx = 0; idx < btc_txn_context->metadata.input_count; idx++) {
    // generate the input digest and respective private key
    status = btc_digest_input(btc_txn_context, idx, buffer);
    memcpy(&t_node, &node, sizeof(HDNode));
    hdnode_private_ckd(&t_node, btc_txn_context->inputs[idx].change_index);
    hdnode_private_ckd(&t_node, btc_txn_context->inputs[idx].address_index);
    hdnode_fill_public_key(&t_node);
    ecdsa_sign_digest(
        curve, t_node.private_key, buffer, signatures[idx].bytes, NULL, NULL);
    signatures[idx].size = btc_sig_to_script_sig(
        signatures[idx].bytes, t_node.public_key, signatures[idx].bytes);
    if (0 == signatures[idx].size || false == status) {
      // early exit as digest could not be calculated
      btc_send_error(ERROR_COMMON_ERROR_UNKNOWN_ERROR_TAG, 1);
      status = false;
      break;
    }
  }
  memzero(&node, sizeof(HDNode));
  memzero(&t_node, sizeof(HDNode));
  memzero(buffer, sizeof(buffer));
  return status;
}

static bool send_script_sig(btc_query_t *query, const scrip_sig_t *sigs) {
  btc_result_t result = init_btc_result(BTC_RESULT_SIGN_TXN_TAG);
  result.sign_txn.which_response = BTC_SIGN_TXN_RESPONSE_SIGNATURE_TAG;

  for (int idx = 0; idx < btc_txn_context->metadata.input_count; idx++) {
    if (!btc_get_query(query, BTC_QUERY_SIGN_TXN_TAG) ||
        !check_which_request(query, BTC_SIGN_TXN_REQUEST_SIGNATURE_TAG)) {
      return false;
    }
    memcpy(
        &result.sign_txn.signature.signature, &sigs[idx], sizeof(scrip_sig_t));
    btc_send_result(&result);
  }
  return true;
}

/*****************************************************************************
 * GLOBAL FUNCTIONS
 *****************************************************************************/

void btc_sign_transaction(btc_query_t *query) {
  btc_txn_context = (btc_txn_context_t *)malloc(sizeof(btc_txn_context_t));
  memzero(btc_txn_context, sizeof(btc_txn_context_t));
  scrip_sig_t signatures[TXN_MAX_INPUTS] = {0};

  if (handle_initiate_query(query) && fetch_transaction_meta(query) &&
      fetch_valid_input(query) && fetch_valid_output(query) &&
      get_user_verification() && sign_input(&signatures[0]) &&
      send_script_sig(query, &signatures[0])) {
    delay_scr_init(ui_text_check_cysync, DELAY_TIME);
  }

  if (NULL != btc_txn_context && NULL != btc_txn_context->inputs) {
    free(btc_txn_context->inputs);
  }
  if (NULL != btc_txn_context && NULL != btc_txn_context->outputs) {
    free(btc_txn_context->outputs);
  }
  if (NULL != btc_txn_context) {
    free(btc_txn_context);
    btc_txn_context = NULL;
  }
}