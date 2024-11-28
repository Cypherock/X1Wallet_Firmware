/**
 * @file    btc_txn_helper_tests.c
 * @author  Cypherock X1 Team
 * @brief   Unit tests for Bitcoin transaction helper functions
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

#include "bip32.h"
#include "btc_app.h"
#include "btc_helpers.h"
#include "btc_priv.h"
#include "btc_script.h"
#include "btc_txn_helpers.h"
#include "curves.h"
#include "ecdsa.h"
#include "unity_fixture.h"
#include "utils.h"

uint32_t get_transaction_weight(const btc_txn_context_t *txn_ctx);

// wrapper function to call 'btc_verify_input'
// The function takes the entire 'raw_txn' and feeds it to
// 'btc_verify_input' in chunks of size CHUNK_SIZE, simulating the sdk
int btc_verify_input_test(btc_sign_txn_input_t *input,
                          uint8_t *raw_txn,
                          int ip_txn_bytes_size) {
  btc_verify_input_t verify_input_data;
  int status = 4;
  memzero(&(verify_input_data), sizeof(btc_verify_input_t));
  verify_input_data.chunk_total = (ip_txn_bytes_size % CHUNK_SIZE == 0)
                                      ? (ip_txn_bytes_size / CHUNK_SIZE)
                                      : (ip_txn_bytes_size / CHUNK_SIZE) + 1;

  uint8_t txn_chunk[CHUNK_SIZE] = {0};
  verify_input_data.size_last_chunk = ip_txn_bytes_size % CHUNK_SIZE;
  int index = 0;
  for (int chunk_var = ip_txn_bytes_size; chunk_var > 0;
       chunk_var -= CHUNK_SIZE) {
    int chvar = (chunk_var >= CHUNK_SIZE) ? CHUNK_SIZE : chunk_var % CHUNK_SIZE;

    for (int i = 0; i < (chvar); i++) {
      txn_chunk[i] = raw_txn[i + (CHUNK_SIZE * index)];
    }

    status = btc_verify_input(txn_chunk, index, &verify_input_data, input);
    if (status != 4) {
      break;
    }
    index++;
  }
  return status;
}

TEST_GROUP(btc_txn_helper_test);

/**
 * @brief Test setup for usb event consumer tests.
 * @details The function populates data in local buffer of USB communication
 * module so that the event getter has an event ready for dispatch for
 * performing tests. buffer of packet(s) of data.
 */
TEST_SETUP(btc_txn_helper_test) {
  g_btc_app = get_btc_app_desc()->app_config;
}

/**
 * @brief Tear down the old test data
 * @details The function will perform cleanup of the current running test and
 * bring the state of execution to a fresh start. This is done by using purge
 * api of usb-event and clearing buffers using usb-comm APIs.
 */
TEST_TEAR_DOWN(btc_txn_helper_test) {
  g_btc_app = NULL;
}

TEST(btc_txn_helper_test, btc_txn_helper_verify_input_p2pk) {
  /* Test data source: rawTxn -
   * https://blockchain.info/rawtx/0437cd7f8525ceed2324359c2d0ba26006d92d856a9c20fa0241106ee5a597c9?format=hex
   * txnElements -
   * https://blockchain.info/rawtx/f4184fc596403b9d638783cf57adfe4c75c605f6356fbc91338530e9831e9e16?format=json
   * Code reference - https://www.blockchain.com/explorer/api/blockchain_api
   */
  uint8_t raw_txn[300] = {0};
  hex_string_to_byte_array(
      "010000000100000000000000000000000000000000000000000000000000000000000000"
      "00ffffffff0704ffff001d0134ffffffff0100f2052a0100000043410411db93e1dcdb8a"
      "016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5cb2e0eaddfb84ccf9744464"
      "f82e160bfa9b8b64f9d4c03f999b8643f656b412a3ac00000000",
      268,
      raw_txn);
  // only fill necessary values
  btc_sign_txn_input_t input = {.prev_output_index = 0,
                                .value = 5000000000,
                                .script_pub_key = {
                                    .size = 67,
                                }};
  hex_string_to_byte_array(
      "0437cd7f8525ceed2324359c2d0ba26006d92d856a9c20fa0241106ee5a597c9",
      64,
      input.prev_txn_hash);
  // Reverse order of txn-id:
  cy_reverse_byte_array(input.prev_txn_hash, sizeof(input.prev_txn_hash));

  hex_string_to_byte_array(
      "410411db93e1dcdb8a016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5cb2e0"
      "eaddfb84ccf9744464f82e160bfa9b8b64f9d4c03f999b8643f656b412a3ac",
      134,
      input.script_pub_key.bytes);

  int status = btc_verify_input_test(&input, raw_txn, 134);

  TEST_ASSERT_EQUAL_INT(0, status);
}

TEST(btc_txn_helper_test, btc_txn_helper_verify_input_p2pk_fail) {
  /* Test data source: rawTxn -
   * https://blockchain.info/rawtx/0437cd7f8525ceed2324359c2d0ba26006d92d856a9c20fa0241106ee5a597c9?format=hex
   * txnElements -
   * https://blockchain.info/rawtx/f4184fc596403b9d638783cf57adfe4c75c605f6356fbc91338530e9831e9e16?format=json
   * Code reference - https://www.blockchain.com/explorer/api/blockchain_api
   */
  uint8_t raw_txn[300] = {0};
  hex_string_to_byte_array(
      "010000000100000000000000000000000000000000000000000000000000000000000000"
      "00ffffffff0704ffff001d0134ffffffff0100f2052a0100000043410411db93e1dcdb8a"
      "016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5cb2e0eaddfb84ccf9744464"
      "f82e160bfa9b8b64f9d4c03f999b8643f656b412a3ac00000000",
      268,
      raw_txn);
  // only fill necessary values
  btc_sign_txn_input_t input = {
      .prev_output_index = 1,    // incorrect index; correct is '0'
      .value = 5000000000,
      .script_pub_key = {
          .size = 67,
      }};
  hex_string_to_byte_array(
      "0437cd7f8525ceed2324359c2d0ba26006d92d856a9c20fa0241106ee5a597c9",
      64,
      input.prev_txn_hash);
  // Reverse order of txn-id:
  // 0437cd7f8525ceed2324359c2d0ba26006d92d856a9c20fa0241106ee5a597c9
  cy_reverse_byte_array(input.prev_txn_hash, sizeof(input.prev_txn_hash));
  hex_string_to_byte_array(
      "410411db93e1dcdb8a016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5cb2e0"
      "eaddfb84ccf9744464f82e160bfa9b8b64f9d4c03f999b8643f656b412a3ac",
      134,
      input.script_pub_key.bytes);

  int status = btc_verify_input_test(&input, raw_txn, 134);

  TEST_ASSERT_EQUAL_INT(1, status);
}

TEST(btc_txn_helper_test, btc_txn_helper_verify_input_p2pkh) {
  /* Test data source: rawTxn -
   * https://blockchain.info/rawtx/eb0e2029310edade8e2a034aea4f0c4a1e243fe2dce67d05f95fddb7ac11bfbe?format=hex
   * txnElements -
   * https://blockchain.info/rawtx/16fbc39570ac5f16c103a39da1920ab3b77ad4f21f3c6d415c745bd6a37097e1?format=json
   * Code reference - https://www.blockchain.com/explorer/api/blockchain_api
   */
  uint8_t raw_txn[300] = {0};
  hex_string_to_byte_array(
      "01000000014da2d059c1c6eb1c66884643f3bfa917cdb182273bf9dd2361db0c1c6bc706"
      "61000000008b483045022100f2522df4a0d2193ee53ad95b698bf502e5874d340b4bb4f8"
      "0720015f2ce87296022061d33d02d6f4a3b131a18328c3b1249e53fa115735c2597e29b0"
      "738a1d5f3f8801410445bd85326dabc1772b4b319e0dc924ef93caf2360a033941e427f0"
      "397b265f3c46e805be40904034880e781ab758a9e67518c624393e2ac14339faa45fafc5"
      "deffffffff0100e1f505000000001976a91412ab8dc588ca9d5787dde7eb29569da63c3a"
      "238c88ac00000000",
      448,
      raw_txn);
  // only fill necessary values
  btc_sign_txn_input_t input = {.prev_output_index = 0,
                                .value = 100000000,
                                .script_pub_key = {
                                    .size = 25,
                                }};
  hex_string_to_byte_array(
      "eb0e2029310edade8e2a034aea4f0c4a1e243fe2dce67d05f95fddb7ac11bfbe",
      64,
      input.prev_txn_hash);
  // Reverse order of txn-id:
  // eb0e2029310edade8e2a034aea4f0c4a1e243fe2dce67d05f95fddb7ac11bfbe
  cy_reverse_byte_array(input.prev_txn_hash, sizeof(input.prev_txn_hash));
  hex_string_to_byte_array("76a91412ab8dc588ca9d5787dde7eb29569da63c3a238c88ac",
                           50,
                           input.script_pub_key.bytes);

  int status = btc_verify_input_test(&input, raw_txn, 224);
  TEST_ASSERT_EQUAL_INT(0, status);
}

TEST(btc_txn_helper_test, btc_txn_helper_verify_input_p2pkh_fail) {
  /* Test data source: rawTxn -
   * https://blockchain.info/rawtx/eb0e2029310edade8e2a034aea4f0c4a1e243fe2dce67d05f95fddb7ac11bfbe?format=hex
   * txnElements -
   * https://blockchain.info/rawtx/16fbc39570ac5f16c103a39da1920ab3b77ad4f21f3c6d415c745bd6a37097e1?format=json
   * Code reference - https://www.blockchain.com/explorer/api/blockchain_api
   */
  uint8_t raw_txn[300] = {0};
  hex_string_to_byte_array(
      "01000000014da2d059c1c6eb1c66884643f3bfa917cdb182273bf9dd2361db0c1c6bc706"
      "61000000008b483045022100f2522df4a0d2193ee53ad95b698bf502e5874d340b4bb4f8"
      "0720015f2ce87296022061d33d02d6f4a3b131a18328c3b1249e53fa115735c2597e29b0"
      "738a1d5f3f8801410445bd85326dabc1772b4b319e0dc924ef93caf2360a033941e427f0"
      "397b265f3c46e805be40904034880e781ab758a9e67518c624393e2ac14339faa45fafc5"
      "deffffffff0100e1f505000000001976a91412ab8dc588ca9d5787dde7eb29569da63c3a"
      "238c88ac00000000",
      448,
      raw_txn);
  // only fill necessary values
  btc_sign_txn_input_t input = {
      .prev_output_index = 0,
      .value = 1000000000,    // invalid value; more by a factor of 10
      .script_pub_key = {
          .size = 25,
      }};
  hex_string_to_byte_array(
      "eb0e2029310edade8e2a034aea4f0c4a1e243fe2dce67d05f95fddb7ac11bfbe",
      64,
      input.prev_txn_hash);
  // Reverse order of txn-id:
  // eb0e2029310edade8e2a034aea4f0c4a1e243fe2dce67d05f95fddb7ac11bfbe
  cy_reverse_byte_array(input.prev_txn_hash, sizeof(input.prev_txn_hash));
  hex_string_to_byte_array("76a91412ab8dc588ca9d5787dde7eb29569da63c3a238c88ac",
                           50,
                           input.script_pub_key.bytes);

  int status = btc_verify_input_test(&input, raw_txn, 224);
  TEST_ASSERT_EQUAL_INT(3, status);
}

TEST(btc_txn_helper_test, btc_txn_helper_verify_input_p2wpkh) {
  /* Test data source: rawTxn -
   * https://blockchain.info/rawtx/21706dfac590a74e7d083ad60e790c3a1775a4818afd7aa3ddf1a3d76dc16b03?format=hex
   * txnElements -
   * https://blockchain.info/rawtx/fcb26cf6235d591b89494398b51746610917fd49c376994fa0ce24fcc383eac0?format=json
   * Code reference - https://www.blockchain.com/explorer/api/blockchain_api
   */
  uint8_t raw_txn[2000] = {0};
  hex_string_to_byte_array(
      "0100000006d572e075a155d3fa334230a691cf085d463215aead3c417f08f931ccbbe6a1"
      "321b0000006b483045022100cdeb3add0650fc8b8351512d6df17fbfc602ec484d8c5c6a"
      "7800d3cd5e71fac60220481616b00629c8c7a81a10397db2942ce21676eb5fd4be20bc44"
      "31754c657f6001210217630c3bfab894e6e7322ae6d104d9b9aff58ae8ce4c30f45e9903"
      "51ba3668a4ffffffff9b4139cb7a02366cd228514a89b46cb55fc8183b03c3f9b8fe583e"
      "fe93f28958420000006b483045022100a19519b0d4af6a5f50301c36d620c7c2b5fea705"
      "7874a184c03db88196933fd4022070e559e34fe80e17a6d9e4b400d183c22af094684004"
      "1220a0965487964b46f301210334f414ce378f5f24a128dbf34a1be8d8a1b863e665f782"
      "99a52f06991be8406dffffffff4b98c9a61ad1b2070ebbcfc2d616824ff3259c5c0d09b8"
      "1f38975446f12e57cb3a0000006b483045022100e9b10baf0226b7394474142b87edcd26"
      "fef74f6fcb6b82833559e95481a51f5802204db8a3f7962f1e731b815f01807ef16b6d48"
      "bd73a40e6534d8e4abc1d72a2554012103e7dcb93f93afdf17b60e00b21f8a283e8140f5"
      "3619970daf7d3b5934230c420dfffffffff63acaa7893f29aa069dbbd6c74bdb9f597d70"
      "3e0f9a99d52b5930edd568ff54000000006b483045022100a3b9578999c1fb5d07c047c9"
      "6569476a73cc8684b1b6f1461cdeaa81de58e631022049cb3e73977605aad472e470a995"
      "9ab913f2b101c268b8cddadd4bca16db4bf10121033011c8839eb82d1c37596bee611cf7"
      "0564d21fb38ce0f4535c4d1d5e53dad958ffffffffd5c0e501e2045ba6fed26aba945e0d"
      "6333d2ca555a60e4c1c920f3e27030bc17400000006b483045022100bbf7e176a402c223"
      "32ab21c3aa0a5ca05bb9efe0d2a3f3797d5174873d4fc5e8022061aba78025eb56e7a8e1"
      "663c68bb63ff0d0d7db0b6fc5064e021994a59cf8c0c012103ca9827e71289cfc3581340"
      "ae413ae7860af88829b05e30083cee496cb9cdff39ffffffff64cca9bfba56c7f2dfb497"
      "53a7f90fe8e2b2121f3d1cd9221a1032425095ed3a8f0000006b483045022100b275537b"
      "a5e33b3511925e68df84d62487640dd813cf179cf65abd2920f4c29f022072523568c14f"
      "50881bd527d352847a343d65f5620a0df6e6683c8a793ff77a240121030ab067dab80cd5"
      "89f30e36d45902e933abc270304829b4e7323ba51695f8d331ffffffff01f0270f000000"
      "0000160014854fe623a8a6a4c76779b57c3895ed2e0962647400000000",
      1858,
      raw_txn);
  // only fill necessary values
  btc_sign_txn_input_t input = {.prev_output_index = 0,
                                .value = 993264,
                                .script_pub_key = {
                                    .size = 22,
                                }};
  hex_string_to_byte_array(
      "21706dfac590a74e7d083ad60e790c3a1775a4818afd7aa3ddf1a3d76dc16b03",
      64,
      input.prev_txn_hash);
  // Reverse order of txn-id:
  // 21706dfac590a74e7d083ad60e790c3a1775a4818afd7aa3ddf1a3d76dc16b03
  cy_reverse_byte_array(input.prev_txn_hash, sizeof(input.prev_txn_hash));
  hex_string_to_byte_array("0014854fe623a8a6a4c76779b57c3895ed2e09626474",
                           44,
                           input.script_pub_key.bytes);

  int status = btc_verify_input_test(&input, raw_txn, 929);
  TEST_ASSERT_EQUAL_INT(0, status);
}

TEST(btc_txn_helper_test, btc_txn_helper_verify_input_p2wpkh_fail) {
  /* Test data source: rawTxn -
   * https://blockchain.info/rawtx/21706dfac590a74e7d083ad60e790c3a1775a4818afd7aa3ddf1a3d76dc16b03?format=hex
   * txnElements -
   * https://blockchain.info/rawtx/fcb26cf6235d591b89494398b51746610917fd49c376994fa0ce24fcc383eac0?format=json
   * Code reference - https://www.blockchain.com/explorer/api/blockchain_api
   */
  uint8_t raw_txn[2000] = {0};
  hex_string_to_byte_array(
      "0100000006d572e075a155d3fa334230a691cf085d463215aead3c417f08f931ccbbe6a1"
      "321b0000006b483045022100cdeb3add0650fc8b8351512d6df17fbfc602ec484d8c5c6a"
      "7800d3cd5e71fac60220481616b00629c8c7a81a10397db2942ce21676eb5fd4be20bc44"
      "31754c657f6001210217630c3bfab894e6e7322ae6d104d9b9aff58ae8ce4c30f45e9903"
      "51ba3668a4ffffffff9b4139cb7a02366cd228514a89b46cb55fc8183b03c3f9b8fe583e"
      "fe93f28958420000006b483045022100a19519b0d4af6a5f50301c36d620c7c2b5fea705"
      "7874a184c03db88196933fd4022070e559e34fe80e17a6d9e4b400d183c22af094684004"
      "1220a0965487964b46f301210334f414ce378f5f24a128dbf34a1be8d8a1b863e665f782"
      "99a52f06991be8406dffffffff4b98c9a61ad1b2070ebbcfc2d616824ff3259c5c0d09b8"
      "1f38975446f12e57cb3a0000006b483045022100e9b10baf0226b7394474142b87edcd26"
      "fef74f6fcb6b82833559e95481a51f5802204db8a3f7962f1e731b815f01807ef16b6d48"
      "bd73a40e6534d8e4abc1d72a2554012103e7dcb93f93afdf17b60e00b21f8a283e8140f5"
      "3619970daf7d3b5934230c420dfffffffff63acaa7893f29aa069dbbd6c74bdb9f597d70"
      "3e0f9a99d52b5930edd568ff54000000006b483045022100a3b9578999c1fb5d07c047c9"
      "6569476a73cc8684b1b6f1461cdeaa81de58e631022049cb3e73977605aad472e470a995"
      "9ab913f2b101c268b8cddadd4bca16db4bf10121033011c8839eb82d1c37596bee611cf7"
      "0564d21fb38ce0f4535c4d1d5e53dad958ffffffffd5c0e501e2045ba6fed26aba945e0d"
      "6333d2ca555a60e4c1c920f3e27030bc17400000006b483045022100bbf7e176a402c223"
      "32ab21c3aa0a5ca05bb9efe0d2a3f3797d5174873d4fc5e8022061aba78025eb56e7a8e1"
      "663c68bb63ff0d0d7db0b6fc5064e021994a59cf8c0c012103ca9827e71289cfc3581340"
      "ae413ae7860af88829b05e30083cee496cb9cdff39ffffffff64cca9bfba56c7f2dfb497"
      "53a7f90fe8e2b2121f3d1cd9221a1032425095ed3a8f0000006b483045022100b275537b"
      "a5e33b3511925e68df84d62487640dd813cf179cf65abd2920f4c29f022072523568c14f"
      "50881bd527d352847a343d65f5620a0df6e6683c8a793ff77a240121030ab067dab80cd5"
      "89f30e36d45902e933abc270304829b4e7323ba51695f8d331ffffffff01f0270f000000"
      "0000160014854fe623a8a6a4c76779b57c3895ed2e0962647400000000",
      1858,
      raw_txn);
  // only fill necessary values
  btc_sign_txn_input_t input = {.prev_output_index = 0,
                                .value = 993264,
                                .script_pub_key = {
                                    .size = 22,
                                }};
  hex_string_to_byte_array(
      // invalid txn hash test. valid txn hash/id:
      // 21706dfac590a74e7d083ad60e790c3a1775a4818afd7aa3ddf1a3d76dc16b03
      "21706dfac590a74e7d083ad60e790c3a1775a4818afd7aa3ddf1a3d76dc16b04",
      64,
      input.prev_txn_hash);
  // Reverse order of txn-id:
  // 21706dfac590a74e7d083ad60e790c3a1775a4818afd7aa3ddf1a3d76dc16b04
  cy_reverse_byte_array(input.prev_txn_hash, sizeof(input.prev_txn_hash));
  hex_string_to_byte_array("0014854fe623a8a6a4c76779b57c3895ed2e09626474",
                           44,
                           input.script_pub_key.bytes);

  int status = btc_verify_input_test(&input, raw_txn, 929);
  TEST_ASSERT_EQUAL_INT(2, status);
}

TEST(btc_txn_helper_test, btc_txn_helper_verify_input_p2wpkh_in_p2sh) {
  /* Test data source: Bip143
   * https://blockstream.info/testnet/tx/b095225e60df2896a4e03fcc4fb2ba9622ee513f34b3e8de45c25a4793a244a2?expand
   */
  int status = 2;
  uint8_t raw_txn[2000] = {0};
  hex_string_to_byte_array(
      "0200000000010258afb1ece76f01c24f4935f453d210518163cb1d0383eaec331b202ebe"
      "b5e389"
      "0000000017160014a76cad25cb569bb47305513ebedd6011dc419deeffffffff2b3682b3"
      "592588"
      "5001f0e321df28d4ac675a9cbbccef2a69533bea7c5e5ad2c40000000017160014a76cad"
      "25cb56"
      "9bb47305513ebedd6011dc419deeffffffff02941100000000000017a914bd7aabdeeef2"
      "11b1bd"
      "ad7218e14fea6c032101c087f22f00000000000017a914eaf97514c5ac1e41e413502e97"
      "ae42eb"
      "f27ace3a870247304402206e038f4712541d699697ed55efc41219df4f244fc72caa5edd"
      "653837"
      "f6555f6f02201cd8ea15b65fda17992abafaed86e066c3271ac16b9c46c54c2192438843"
      "dd0401"
      "21029f75e1ef6b04e004a308b1f59215a8a3a5b7958bbcf184cc24ba7ab6574448780248"
      "304502"
      "2100d15ce61648edc28b8b5a3531b80a1e8fc3b3eebe7d3fc4ca962cb04afc770dda0220"
      "7c7eaf8"
      "82d7fac45d2752f20e48d2f896715cbc5a3b0f5de3e19fea0da99beac0121029f75e1ef6"
      "b04e004"
      "a308b1f59215a8a3a5b7958bbcf184cc24ba7ab65744487800000000",
      838,
      raw_txn);
  // only fill necessary values
  btc_txn_input_t input[] = {{
                                 .value = 4500,
                                 .prev_output_index = 0x00000000,
                                 .script_pub_key = {.size = 23},
                             },
                             {
                                 .value = 12274,
                                 .prev_output_index = 0x00000001,
                                 .script_pub_key = {.size = 23},
                             }};

  hex_string_to_byte_array(
      "b095225e60df2896a4e03fcc4fb2ba9622ee513f34b3e8de45c25a4793a244a2",
      64,
      input[0].prev_txn_hash);
  // Reverse order of txn-id:
  // A244A293475AC245DEE8B3343F51EE2296BAB24FCC3FE0A49628DF605E2295B0
  cy_reverse_byte_array(input[0].prev_txn_hash, sizeof(input[0].prev_txn_hash));

  hex_string_to_byte_array(
      "b095225e60df2896a4e03fcc4fb2ba9622ee513f34b3e8de45c25a4793a244a2",
      64,
      input[1].prev_txn_hash);
  // Reverse order of txn-id:
  // A244A293475AC245DEE8B3343F51EE2296BAB24FCC3FE0A49628DF605E2295B0
  cy_reverse_byte_array(input[1].prev_txn_hash, sizeof(input[1].prev_txn_hash));

  hex_string_to_byte_array("a914bd7aabdeeef211b1bdad7218e14fea6c032101c087",
                           46,
                           input[0].script_pub_key.bytes);
  hex_string_to_byte_array("a914eaf97514c5ac1e41e413502e97ae42ebf27ace3a87",
                           46,
                           input[1].script_pub_key.bytes);

  status = btc_verify_input_test(input, raw_txn, 419);
  TEST_ASSERT_EQUAL_INT(0, status);
  status = btc_verify_input_test(input + 1, raw_txn, 419);
  TEST_ASSERT_EQUAL_INT(0, status);
}

TEST(btc_txn_helper_test, btc_txn_helper_verify_input_p2wpkh_in_p2sh_fail) {
  /* Test data source: Bip143
   * https://blockstream.info/testnet/tx/b095225e60df2896a4e03fcc4fb2ba9622ee513f34b3e8de45c25a4793a244a2?expand
   */
  int status = 2;
  uint8_t raw_txn[2000] = {0};
  hex_string_to_byte_array(
      "0200000000010258afb1ece76f01c24f4935f453d210518163cb1d0383eaec331b202ebe"
      "b5e389"
      "0000000017160014a76cad25cb569bb47305513ebedd6011dc419deeffffffff2b3682b3"
      "592588"
      "5001f0e321df28d4ac675a9cbbccef2a69533bea7c5e5ad2c40000000017160014a76cad"
      "25cb56"
      "9bb47305513ebedd6011dc419deeffffffff02941100000000000017a914bd7aabdeeef2"
      "11b1bd"
      "ad7218e14fea6c032101c087f22f00000000000017a914eaf97514c5ac1e41e413502e97"
      "ae42eb"
      "f27ace3a870247304402206e038f4712541d699697ed55efc41219df4f244fc72caa5edd"
      "653837"
      "f6555f6f02201cd8ea15b65fda17992abafaed86e066c3271ac16b9c46c54c2192438843"
      "dd0401"
      "21029f75e1ef6b04e004a308b1f59215a8a3a5b7958bbcf184cc24ba7ab6574448780248"
      "304502"
      "2100d15ce61648edc28b8b5a3531b80a1e8fc3b3eebe7d3fc4ca962cb04afc770dda0220"
      "7c7eaf8"
      "82d7fac45d2752f20e48d2f896715cbc5a3b0f5de3e19fea0da99beac0121029f75e1ef6"
      "b04e004"
      "a308b1f59215a8a3a5b7958bbcf184cc24ba7ab65744487800000000",
      838,
      raw_txn);
  // only fill necessary values
  btc_txn_input_t input[] = {{
                                 .value = 4500,
                                 .prev_output_index = 0x00000000,
                                 .script_pub_key = {.size = 23},
                             },
                             {
                                 .value = 12274,
                                 .prev_output_index = 0x00000001,
                                 .script_pub_key = {.size = 23},
                             }};

  hex_string_to_byte_array(
      // invalid txn hash test. valid txn hash/id:
      // b095225e60df2896a4e03fcc4fb2ba9622ee513f34b3e8de45c25a4793a244a2
      "b095225e60df2896a4e03fcc4fb2ba9622ee513f34b3e8de45c25a4793a244a3",
      64,
      input[0].prev_txn_hash);
  // Reverse order of txn-id:
  // A344A293475AC245DEE8B3343F51EE2296BAB24FCC3FE0A49628DF605E2295B0
  cy_reverse_byte_array(input[0].prev_txn_hash, sizeof(input[0].prev_txn_hash));

  hex_string_to_byte_array(
      // invalid txn hash test. valid txn hash/id:
      // b095225e60df2896a4e03fcc4fb2ba9622ee513f34b3e8de45c25a4793a244a2
      "b095225e60df2896a4e03fcc4fb2ba9622ee513f34b3e8de45c25a4793a244a3",
      64,
      input[1].prev_txn_hash);
  // Reverse order of txn-id:
  // A344A293475AC245DEE8B3343F51EE2296BAB24FCC3FE0A49628DF605E2295B0
  cy_reverse_byte_array(input[1].prev_txn_hash, sizeof(input[1].prev_txn_hash));

  hex_string_to_byte_array("a914bd7aabdeeef211b1bdad7218e14fea6c032101c087",
                           46,
                           input[0].script_pub_key.bytes);
  hex_string_to_byte_array("a914eaf97514c5ac1e41e413502e97ae42ebf27ace3a87",
                           46,
                           input[1].script_pub_key.bytes);

  status = btc_verify_input_test(input, raw_txn, 419);
  TEST_ASSERT_EQUAL_INT(2, status);
  status = btc_verify_input_test(input + 1, raw_txn, 419);
  TEST_ASSERT_EQUAL_INT(2, status);
}

/* FIX: Required to fix the hardcoded value of 106 (2 + 33 + 71) since the
 * signature part of the script can vary (71 bytes | 72 bytes | 73 bytes).
 * Check the get_transaction_weight function. */
static inline uint32_t fix_legacy_weight(uint32_t legacy_weight,
                                         uint32_t input_count,
                                         uint32_t script_sig_size) {
  return legacy_weight +
         ((int32_t)script_sig_size - EXPECTED_SCRIPT_SIG_SIZE * input_count) *
             4;
}

TEST(btc_txn_helper_test, btc_txn_helper_transaction_weight_legacy1) {
  /* Test data source:
   * https://blockchain.info/rawtx/ee6507696b0691651355797c77cf4a6e2a0a30540784c3fd4f45f92ff890b0c1?format=json
   */
  btc_txn_context_t txn_ctx = {
      .metadata =
          {
              .version = 1,
              .output_count = 2,
              .input_count = 1,
              .locktime = 0,
              .sighash = 1,
          },
      .inputs = NULL,
      .outputs = NULL,
  };
  txn_ctx.inputs = (btc_txn_input_t *)malloc(sizeof(btc_txn_input_t));
  txn_ctx.outputs =
      (btc_sign_txn_output_t *)malloc(sizeof(btc_sign_txn_output_t) * 2);

  hex_string_to_byte_array("76a9149e8bf5383534bbcdecbf2f25e1c61d50ccab94de88ac",
                           50,
                           txn_ctx.inputs[0].script_pub_key.bytes);
  /* Input 0 ScriptSig:
   * 47304402205291ec6f7870d49158d3e03cd7f3cdf33044cbd248d63f73ed46f4e83bac173f022053639907c363acd4c2c7774616417a4bb2c0817e49f630841e9329970f7d5d01012103c5d52e46f6a9312127d552f201ec7cde3c8fae420731bc198c650ac1f685ae8b
   */
  uint32_t script_sig_size = 106;

  /* 76a914f9f6a393d59b793a421b5f995bb09da767ec4f6588ac */
  txn_ctx.outputs[0].script_pub_key.size = 25;
  /* 76a9148d10806ee8786726bf2d2b03b7168fbd96f1475088ac */
  txn_ctx.outputs[1].script_pub_key.size = 25;

  uint32_t weight = get_transaction_weight(&txn_ctx);
  uint32_t fixed_weight =
      fix_legacy_weight(weight, txn_ctx.metadata.input_count, script_sig_size);
  TEST_ASSERT_EQUAL_UINT(900, fixed_weight);

  free(txn_ctx.inputs);
  free(txn_ctx.outputs);
}

TEST(btc_txn_helper_test, btc_txn_helper_transaction_weight_legacy2) {
  /* Test data source:
   * https://blockchain.info/rawtx/c3711ec5011ab5ad86a5193094b51146263290f6cd0434dcf44c3a7885d9e00d?format=json
   */
  btc_txn_context_t txn_ctx = {
      .metadata =
          {
              .version = 1,
              .output_count = 2,
              .input_count = 1,
              .locktime = 0,
              .sighash = 1,
          },
      .inputs = NULL,
      .outputs = NULL,
  };
  txn_ctx.inputs = (btc_txn_input_t *)malloc(sizeof(btc_txn_input_t));
  txn_ctx.outputs =
      (btc_sign_txn_output_t *)malloc(sizeof(btc_sign_txn_output_t) * 2);

  hex_string_to_byte_array("76a914f9f6a393d59b793a421b5f995bb09da767ec4f6588ac",
                           50,
                           txn_ctx.inputs[0].script_pub_key.bytes);
  /* Input 0 ScriptSig:
   * 493046022100cacdac51a47bdd90c9c2f11450b929f131c0433bb6f651a333611d2e01cb2da9022100de25c004529df921fc0e181fcd069b75851f6791999512bdc18632aa0e5fd641012103bd32d9b96614bbc1efb50cfc78b19430ef1297fe68ae8a276b35f46e097440d4
   */
  uint32_t script_sig_size = 108;

  /* 76a914bcf8d79a438f3fb5dac48074811c452761b9479a88ac */
  txn_ctx.outputs[0].script_pub_key.size = 25;
  /* 76a914699dbaa9e46b869021b3d567cbb3e8e6915ecdd288ac */
  txn_ctx.outputs[1].script_pub_key.size = 25;

  uint32_t weight = get_transaction_weight(&txn_ctx);
  uint32_t fixed_weight =
      fix_legacy_weight(weight, txn_ctx.metadata.input_count, script_sig_size);
  TEST_ASSERT_EQUAL_UINT(908, fixed_weight);

  free(txn_ctx.inputs);
  free(txn_ctx.outputs);
}

/* FIX: Required to fix the hardcoded value of 106 (2 + 33 + 71) since the
 * signature part of the witness can vary (71 bytes | 72 bytes | 73 bytes).
 * Check the get_transaction_weight function. */
static inline uint32_t fix_segwit_weight(uint32_t segwit_weight,
                                         uint32_t input_count,
                                         uint32_t witness_size) {
  return segwit_weight - EXPECTED_SCRIPT_SIG_SIZE * input_count + witness_size;
}

TEST(btc_txn_helper_test, btc_txn_helper_transaction_weight_segwit1) {
  /* Test data source:
   * https://blockchain.info/rawtx/55ab320da6636eff54126ca1fcfab8db76b14ad35690f4e6092772655313b80f?format=json
   */
  btc_txn_context_t txn_ctx = {
      .metadata =
          {
              .version = 1,
              .output_count = 2,
              .input_count = 1,
              .locktime = 0,
              .sighash = 1,
          },
      .inputs = NULL,
      .outputs = NULL,
  };
  txn_ctx.inputs = (btc_txn_input_t *)malloc(sizeof(btc_txn_input_t));
  txn_ctx.outputs =
      (btc_sign_txn_output_t *)malloc(2 * sizeof(btc_sign_txn_output_t));

  /* FIX: store the size for witnesses here due to the absence of designated
   * struct fields */
  uint32_t witness_size = 1; /* number of witness components - 1 byte */
  witness_size += 1;         /* size of first component      - 1 byte */
  witness_size += 71;        /* first component */
  witness_size += 1;         /* size of second component     - 1 byte */
  witness_size += 33;        /* second component */
  /* Witness Components
   * ==================
   * Signature:
   * 3044022061cf7d5e42cc8e55a24a315c0a4b589a62ddb5aa3fc730ea00d765d5984977a8022034a69e38e023b3b1f37f52d305ccf816e2ed8a07c855d09d89b3bce10f9aa6d001
   * PubKey:
   * 032959ac5254ec45ff1a0dee1994b33489f6ed670f6db5eb83fca85b544bc78688
   */

  /* fill in relevant values */
  hex_string_to_byte_array("00147c343c768adcb9b01d32e0ab2d5bb4b9657053d9",
                           44,
                           txn_ctx.inputs[0].script_pub_key.bytes);
  /* 76a9149adeade91a046e530e0b162686cb281ce4d9a70188ac */
  txn_ctx.outputs[0].script_pub_key.size = 25;
  /* 00147c343c768adcb9b01d32e0ab2d5bb4b9657053d9 */
  txn_ctx.outputs[1].script_pub_key.size = 22;

  uint32_t txn_weight = get_transaction_weight(&txn_ctx);
  uint32_t fixed_weight =
      fix_segwit_weight(txn_weight, txn_ctx.metadata.input_count, witness_size);

  TEST_ASSERT_EQUAL_UINT(573, fixed_weight);

  free(txn_ctx.inputs);
  free(txn_ctx.outputs);
}

TEST(btc_txn_helper_test, btc_txn_helper_transaction_weight_segwit2) {
  /* Test data source:
   * https://blockchain.info/rawtx/f1457142e99b3ce45a80565237732d8f4ffd251fb70d8283fd7bd6159154e530?format=json
   */
  btc_txn_context_t txn_ctx = {
      .metadata =
          {
              .version = 1,
              .output_count = 1,
              .input_count = 1,
              .locktime = 0,
              .sighash = 1,
          },
      .inputs = NULL,
      .outputs = NULL,
  };
  txn_ctx.inputs = (btc_txn_input_t *)malloc(sizeof(btc_txn_input_t));
  txn_ctx.outputs =
      (btc_sign_txn_output_t *)malloc(sizeof(btc_sign_txn_output_t));
  btc_txn_input_t *input = &txn_ctx.inputs[0];
  btc_sign_txn_output_t *output = &txn_ctx.outputs[0];

  /* FIX: store the size for witnesses here due to the absence of designated
   * struct fields */
  uint32_t witness_size = 1; /* number of witness components - 1 byte */
  witness_size += 1;         /* size of first component      - 1 byte */
  witness_size += 71;        /* first component */
  witness_size += 1;         /* size of second component     - 1 byte */
  witness_size += 33;        /* second component */
  /* Witness Components
   * ==================
   * Signature:
   * 30440220617a8fe692258e918007d3c467cf5a7d12d8f218a3f68181e3aab61c9b521e4702202b5997d5cd125135e8353e9be15b27a8a0f639a7017b16b9faf948c123ac408f01
   * PubKey:
   * 0323b4774cd51989d43d59ac61b269b6bca28545e6ded8ff1959fc81f8f9fb8733
   */

  /* fill in relevant values */
  hex_string_to_byte_array("00149031c2a9996e57eb787967e794358e823595b666",
                           44,
                           input->script_pub_key.bytes);
  /* 76a914d2192be350c2e4b16d4905d0c356080c331e34de88ac */
  output->script_pub_key.size = 25;

  uint32_t txn_weight = get_transaction_weight(&txn_ctx);
  uint32_t fixed_weight =
      fix_segwit_weight(txn_weight, txn_ctx.metadata.input_count, witness_size);

  TEST_ASSERT_EQUAL_UINT(449, fixed_weight);

  free(txn_ctx.inputs);
  free(txn_ctx.outputs);
}

TEST(btc_txn_helper_test, btc_txn_helper_transaction_weight_mixed) {
}

TEST(btc_txn_helper_test, btc_txn_helper_format_value_0sat) {
  uint64_t value_in_sat = 0;
  char msg[100] = "";
  const char expected_msg[] = "0 BTC";
  format_value(value_in_sat, msg, sizeof(msg));
  TEST_ASSERT_EQUAL_STRING(expected_msg, msg);
}

TEST(btc_txn_helper_test, btc_txn_helper_format_value_1sat) {
  uint64_t value_in_sat = 1;
  char msg[100] = "";
  const char expected_msg[] = "0.00000001 BTC";
  format_value(value_in_sat, msg, sizeof(msg));
  TEST_ASSERT_EQUAL_STRING(expected_msg, msg);
}

TEST(btc_txn_helper_test, btc_txn_helper_format_value_1001sat) {
  uint64_t value_in_sat = 1001;
  char msg[100] = "";
  const char expected_msg[] = "0.00001001 BTC";
  format_value(value_in_sat, msg, sizeof(msg));
  TEST_ASSERT_EQUAL_STRING(expected_msg, msg);
}

TEST(btc_txn_helper_test, btc_txn_helper_format_value_1btc) {
  uint64_t value_in_sat = SATOSHI_PER_BTC;
  char msg[100] = "";
  const char expected_msg[] = "1 BTC";
  format_value(value_in_sat, msg, sizeof(msg));
  TEST_ASSERT_EQUAL_STRING(expected_msg, msg);
}

TEST(btc_txn_helper_test, btc_script_check_script_address) {
  uint8_t script_pub[22] = {0};
  uint8_t public_key[33] = {0};

  hex_string_to_byte_array(
      "00144e0fd42c1c6f2597b14042037557b516187cfa38", 44, script_pub);
  hex_string_to_byte_array(
      "02cc2dd74781ac8ee9d931bba9783abac2c1f20d503d5f6df1e53f84e913a4a266",
      66,
      public_key);
  TEST_ASSERT_TRUE(
      btc_check_script_address(script_pub, sizeof(script_pub), public_key));
}

TEST(btc_txn_helper_test, btc_script_check_p2sh_p2pwkh_script_address) {
  uint8_t script_pub[23] = {0};
  uint8_t public_key[33] = {0};

  hex_string_to_byte_array(
      "a914336caa13e08b96080a32b5d818d59b4ab3b3674287", 46, script_pub);
  hex_string_to_byte_array(
      "03a1af804ac108a8a51782198c2d034b28bf90c8803f5a53f76276fa69a4eae77f",
      66,
      public_key);

  TEST_ASSERT_TRUE(
      btc_check_script_address(script_pub, sizeof(script_pub), public_key));
}

TEST(btc_txn_helper_test, btc_txn_helper_p2pkh_digest_1_2) {
  uint8_t calculated_digest[32] = {0};
  uint8_t expected_digest[32] = {0};
  btc_txn_input_t input = {
      .prev_txn_hash = {0},
      .value = 11014713900,
      .prev_output_index = 0,
      .script_pub_key = {.size = 25},
      .change_index = 0,
      .address_index = 0,
      .sequence = UINT32_MAX,
  };
  btc_sign_txn_output_t outputs[] = {
      {
          .value = 1100000000,
          .script_pub_key = {.size = 25},
          .is_change = false,
          .has_changes_index = false,
      },
      {
          .value = 9883316624,
          .script_pub_key = {.size = 25},
          .is_change = true,
          .has_changes_index = true,
          .changes_index = 0,
      },
  };
  btc_txn_context_t context = {
      .init_info = {.derivation_path_count = 3,
                    .derivation_path = {0x80000000 + 84,
                                        0x80000000,
                                        0x80000000}},
      .metadata = {.version = 2,
                   .input_count = 1,
                   .output_count = 2,
                   .sighash = 1,
                   .locktime = 0},
      .segwit_cache = {0},
      .change_output_idx = 1,
  };
  context.inputs = &input;
  context.outputs = outputs;

  hex_string_to_byte_array(
      "630ff449462972b58a3e79d7117ddbc87df95b572185fbaea94282bd7f15c5e7",
      64,
      input.prev_txn_hash);
  hex_string_to_byte_array("76a91448f551693d43698002e0fe9f514aecf6f94f75f688ac",
                           50,
                           input.script_pub_key.bytes);
  hex_string_to_byte_array("76a914e05d7d7e46ff0ad8b53091a4d2edc69b60f251b888ac",
                           50,
                           outputs[0].script_pub_key.bytes);
  hex_string_to_byte_array("76a914a4695d02b19af59cdc2a524ce34eca9af5e1353688ac",
                           50,
                           outputs[1].script_pub_key.bytes);
  hex_string_to_byte_array(
      "7F0CEFA35F4EF744AC73916A1F7B4465FA8FF028C09048E2C043BE0838963A77",
      64,
      expected_digest);
  btc_digest_input(&context, 0, calculated_digest);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_digest, calculated_digest, 32);
}

TEST(btc_txn_helper_test, btc_txn_helper_p2wpkh_digest_1_2) {
  uint8_t calculated_digest[32] = {0};
  uint8_t expected_digest[32] = {0};
  btc_segwit_cache_t expected_cache = {.filled = true};
  btc_txn_input_t input = {
      .prev_txn_hash = {0},
      .value = 76425,
      .prev_output_index = 0,
      .script_pub_key = {.size = 22},
      .change_index = 0,
      .address_index = 1,
      .sequence = UINT32_MAX,
  };
  btc_sign_txn_output_t outputs[] = {
      {
          .value = 10000,
          .script_pub_key = {.size = 22},
          .is_change = false,
          .has_changes_index = false,
      },
      {
          .value = 64165,
          .script_pub_key = {.size = 22},
          .is_change = true,
          .has_changes_index = true,
          .changes_index = 0,
      },
  };
  btc_txn_context_t context = {
      .init_info = {.derivation_path_count = 3,
                    .derivation_path = {0x80000000 + 84,
                                        0x80000000,
                                        0x80000000}},
      .metadata = {.version = 2,
                   .input_count = 1,
                   .output_count = 2,
                   .sighash = 1,
                   .locktime = 0},
      .segwit_cache = {0},
      .change_output_idx = 1,
  };
  context.inputs = &input;
  context.outputs = outputs;

  hex_string_to_byte_array(
      "fa9a08bbb86c80da7944a231240a85e9d46eb2315b6ba9989039247471706bfc",
      64,
      input.prev_txn_hash);
  hex_string_to_byte_array("001423e992c8c08db39037f9e3ab1827e279213ad953",
                           44,
                           input.script_pub_key.bytes);
  hex_string_to_byte_array("00141013ffc18eb488597d5a77c3c17a97341106681e",
                           44,
                           outputs[0].script_pub_key.bytes);
  hex_string_to_byte_array("00147633be4e56de62f4a9b083e5c4a37eeccbba359c",
                           44,
                           outputs[1].script_pub_key.bytes);
  hex_string_to_byte_array(
      "A2BD84FFC6731B976672BEE4BA8710EA5DA4D46A2B4555AE44913FD1C3D5E7A9",
      64,
      expected_cache.hash_prevouts);
  hex_string_to_byte_array(
      "3BB13029CE7B1F559EF5E747FCAC439F1455A2EC7C5F09B72290795E70665044",
      64,
      expected_cache.hash_sequence);
  hex_string_to_byte_array(
      "236FE9B23372D09CC0514E7DD14F137136ED0E211C3FED8E9BB6201C028F0DAD",
      64,
      expected_cache.hash_outputs);
  hex_string_to_byte_array(
      "120D9BC5E49C75F2C32477AF519ED986FE3DEA6DFA90E29506E00D5949722564",
      64,
      expected_digest);
  btc_segwit_init_cache(&context);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      expected_cache.hash_prevouts, context.segwit_cache.hash_prevouts, 32);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      expected_cache.hash_sequence, context.segwit_cache.hash_sequence, 32);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      expected_cache.hash_outputs, context.segwit_cache.hash_outputs, 32);
  btc_digest_input(&context, 0, calculated_digest);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_digest, calculated_digest, 32);
}

TEST(btc_txn_helper_test, btc_txn_helper_p2wpkh_digest_2_2) {
}

TEST(btc_txn_helper_test, btc_txn_helper_p2wpkh_in_p2sh_digest_1_2) {
  uint8_t calculated_digest[32] = {0};
  uint8_t expected_digest[32] = {0};
  btc_segwit_cache_t expected_cache = {.filled = true};
  btc_txn_input_t input = {
      .prev_txn_hash = {0},
      .value = 1000000000,
      .prev_output_index = 0x00000001,
      .script_pub_key = {.size = 22},
      .change_index = 0,
      .address_index = 1,
      .sequence = 0xFFFFFFFe,
  };
  btc_sign_txn_output_t outputs[] = {
      {
          .value = 0x000000000bebb4b8,
          .script_pub_key = {.size = 25},
          .is_change = false,
          .has_changes_index = false,
      },
      {
          .value = 0x000000002faf0800,
          .script_pub_key = {.size = 25},
          .is_change = false,
          .has_changes_index = false,
      },
  };
  btc_txn_context_t context = {
      .init_info = {.derivation_path_count = 3,
                    .derivation_path = {0x80000000 + 49,
                                        0x80000000,
                                        0x80000000}},
      .metadata = {.version = 0x00000001,
                   .input_count = 0x01,
                   .output_count = 0x02,
                   .sighash = 0x00000001,
                   .locktime = 0x00000492},
      .segwit_cache = {0},
      .change_output_idx = 1,
  };

  context.inputs = &input;
  context.outputs = outputs;

  hex_string_to_byte_array(
      "db6b1b20aa0fd7b23880be2ecbd4a98130974cf4748fb66092ac4d3ceb1a5477",
      64,
      input.prev_txn_hash);
  hex_string_to_byte_array("001479091972186c449eb1ded22b78e40d009bdf0089",
                           44,
                           input.script_pub_key.bytes);
  hex_string_to_byte_array("76a914a457b684d7f0d539a46a45bbc043f35b59d0d96388ac",
                           50,
                           outputs[0].script_pub_key.bytes);
  hex_string_to_byte_array("76a914fd270b1ee6abcaea97fea7ad0402e8bd8ad6d77c88ac",
                           50,
                           outputs[1].script_pub_key.bytes);
  hex_string_to_byte_array(
      "b0287b4a252ac05af83d2dcef00ba313af78a3e9c329afa216eb3aa2a7b4613a",
      64,
      expected_cache.hash_prevouts);
  hex_string_to_byte_array(
      "18606b350cd8bf565266bc352f0caddcf01e8fa789dd8a15386327cf8cabe198",
      64,
      expected_cache.hash_sequence);
  hex_string_to_byte_array(
      "de984f44532e2173ca0d64314fcefe6d30da6f8cf27bafa706da61df8a226c83",
      64,
      expected_cache.hash_outputs);
  hex_string_to_byte_array(
      "64f3b0f4dd2bb3aa1ce8566d220cc74dda9df97d8490cc81d89d735c92e59fb6",
      64,
      expected_digest);

  btc_segwit_init_cache(&context);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      expected_cache.hash_prevouts, context.segwit_cache.hash_prevouts, 32);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      expected_cache.hash_sequence, context.segwit_cache.hash_sequence, 32);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      expected_cache.hash_outputs, context.segwit_cache.hash_outputs, 32);

  btc_digest_input(&context, 0, calculated_digest);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_digest, calculated_digest, 32);
}

TEST(btc_txn_helper_test, btc_txn_helper_p2wpkh_in_p2sh_digest_2_2) {
  bool status = false;
  uint8_t calculated_digest[32] = {0};
  uint8_t expected_digest[2][32] = {0};
  btc_segwit_cache_t expected_cache = {.filled = true};
  btc_txn_input_t input[] = {{
                                 .prev_txn_hash = {0},
                                 .value = 8650,
                                 .prev_output_index = 0x00000000,
                                 .script_pub_key = {.size = 22},
                                 .address_index = 0,
                                 .sequence = 4294967295,
                             },
                             {
                                 .prev_txn_hash = {0},
                                 .value = 18123,
                                 .prev_output_index = 0x00000000,
                                 .script_pub_key = {.size = 22},
                                 .address_index = 0,
                                 .sequence = 4294967295,
                             }};
  btc_sign_txn_output_t outputs[] = {
      {
          .value = 4500,
          .script_pub_key = {.size = 23},
          .is_change = false,
          .has_changes_index = false,
      },
      {
          .value = 12274,
          .script_pub_key = {.size = 23},
          .is_change = false,
          .has_changes_index = false,
      },
  };
  btc_txn_context_t context = {
      .init_info = {.derivation_path_count = 3,
                    .derivation_path = {0x80000000 + 49,
                                        0x80000001,
                                        0x80000000}},
      .metadata = {.version = 0x00000002,
                   .input_count = 0x02,
                   .output_count = 0x02,
                   .sighash = 0x01,
                   .locktime = 0},
      .segwit_cache = {0},
  };

  context.inputs = &input;
  context.outputs = outputs;

  hex_string_to_byte_array(
      "58afb1ece76f01c24f4935f453d210518163cb1d0383eaec331b202ebeb5e389",
      64,
      input[0].prev_txn_hash);
  hex_string_to_byte_array("0014a76cad25cb569bb47305513ebedd6011dc419dee",
                           44,
                           input[0].script_pub_key.bytes);
  hex_string_to_byte_array(
      "2b3682b35925885001f0e321df28d4ac675a9cbbccef2a69533bea7c5e5ad2c4",
      64,
      input[1].prev_txn_hash);
  hex_string_to_byte_array("0014a76cad25cb569bb47305513ebedd6011dc419dee",
                           44,
                           input[1].script_pub_key.bytes);
  /*outputs*/
  hex_string_to_byte_array("a914bd7aabdeeef211b1bdad7218e14fea6c032101c087",
                           46,
                           outputs[0].script_pub_key.bytes);
  hex_string_to_byte_array("a914eaf97514c5ac1e41e413502e97ae42ebf27ace3a87",
                           46,
                           outputs[1].script_pub_key.bytes);
  hex_string_to_byte_array(
      "ce9186e6b6b4ce2ed0d42bdd2ae8a0003188b2b5070139eb53d1bd49ade831b7",
      64,
      expected_cache.hash_prevouts);
  hex_string_to_byte_array(
      "752adad0a7b9ceca853768aebb6965eca126a62965f698a0c1bc43d83db632ad",
      64,
      expected_cache.hash_sequence);
  hex_string_to_byte_array(
      "c42e53e6e72da4a8068e3115134201ebdcd8567fcaedaffc3169ab673ef4f0f9",
      64,
      expected_cache.hash_outputs);

  btc_segwit_init_cache(&context);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      expected_cache.hash_prevouts, context.segwit_cache.hash_prevouts, 32);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      expected_cache.hash_sequence, context.segwit_cache.hash_sequence, 32);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(
      expected_cache.hash_outputs, context.segwit_cache.hash_outputs, 32);

  /* digest input 0*/
  hex_string_to_byte_array(
      "84b6c444101a5d7d41c521b1034bf2c82afb19d45fe2a4fc6bdfae4edd720334",
      64,
      expected_digest[0]);

  status = btc_digest_input(&context, 0, calculated_digest);
  TEST_ASSERT_TRUE(status);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_digest[0], calculated_digest, 32);

  /* digest input 1*/
  hex_string_to_byte_array(
      "7503b14676ff1de1d85819f31e43b96a5f400c273e7f143d8b05aa32e261a2b1",
      64,
      expected_digest[1]);

  status = btc_digest_input(&context, 1, calculated_digest);
  TEST_ASSERT_TRUE(status);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_digest[1], calculated_digest, 32);
}

TEST(btc_txn_helper_test, btc_txn_helper_get_fee) {
  /* Test data source:
   * https://blockchain.info/rawtx/b77a9ff6738877d4eb2d300b1c0ec7ca4d14353e8d501d55139019609ca2e4e5?format=json
   */
  btc_txn_context_t txn_ctx = {
      .metadata =
          {
              .version = 1,
              .output_count = 2,
              .input_count = 3,
              .locktime = 0,
              .sighash = 1,
          },
      .inputs = NULL,
      .outputs = NULL,
  };
  txn_ctx.inputs = (btc_txn_input_t *)malloc(3 * sizeof(btc_txn_input_t));
  txn_ctx.outputs =
      (btc_sign_txn_output_t *)malloc(2 * sizeof(btc_sign_txn_output_t));

  txn_ctx.inputs[0].value = 5136;
  txn_ctx.inputs[1].value = 336366;
  txn_ctx.inputs[2].value = 4682;

  txn_ctx.outputs[0].value = 2900;
  txn_ctx.outputs[1].value = 341352;

  uint64_t fee = 0;
  TEST_ASSERT_EQUAL_UINT(true, btc_get_txn_fee(&txn_ctx, &fee));
  TEST_ASSERT_EQUAL_UINT(1932, fee);

  free(txn_ctx.inputs);
  free(txn_ctx.outputs);
}

TEST(btc_txn_helper_test, btc_txn_helper_get_fee_overspend) {
  /* overspent transaction */

  btc_txn_context_t txn_ctx = {
      .metadata =
          {
              .version = 1,
              .output_count = 2,
              .input_count = 1,
              .locktime = 0,
              .sighash = 1,
          },
      .inputs = NULL,
      .outputs = NULL,
  };
  txn_ctx.inputs = (btc_txn_input_t *)malloc(1 * sizeof(btc_txn_input_t));
  txn_ctx.outputs =
      (btc_sign_txn_output_t *)malloc(2 * sizeof(btc_sign_txn_output_t));

  txn_ctx.inputs[0].value = 5000;

  txn_ctx.outputs[0].value = 4999;
  txn_ctx.outputs[1].value = 2;

  uint64_t fee = 0;
  TEST_ASSERT_EQUAL_UINT(false, btc_get_txn_fee(&txn_ctx, &fee));

  free(txn_ctx.inputs);
  free(txn_ctx.outputs);
}
