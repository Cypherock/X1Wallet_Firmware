/**
 * @file    main.c
 * @author  Cypherock X1 Team
 * @brief   Main source file.
 *          Entry point to the application.
 * @copyright Copyright (c) 2022 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 *target=_blank>https://mitcc.org/</a>
 *
 ******************************************************************************
 * @attention
 *
 * (c) Copyright 2022 by HODL TECH PTE LTD
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
/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain"       \
                            issue*/
#include "application_startup.h"
#include "board.h"
#include "logger.h"
#include "onboarding.h"
#include "starknet_crypto.h"
#include "starknet_helpers.h"
#include "starknet_poseidon.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys_state.h"
#include "time.h"
#include "ui_logo.h"

#define RUN_ENGINE
#ifdef RUN_ENGINE
#include "core_flow_init.h"
#endif /* RUN_ENGINE */

#if USE_SIMULATOR == 0
#include "cmsis_gcc.h"
#include "main.h"
#endif    // USE_SIMULATOR

#if USE_SIMULATOR == 1
#ifdef _WIN32
#define main SDL_main
#endif
#include "sim_usb.h"
extern lv_indev_t *indev_keypad;
#endif
extern Counter counter;
extern bool main_app_ready;

#if USE_SIMULATOR == 1

/*On OSX SDL needs different handling*/
#if defined(__APPLE__) && defined(TARGET_OS_MAC)
#if __APPLE__ && TARGET_OS_MAC
#define SDL_APPLE
#endif
#endif

static int tick_thread(void *data);
static void memory_monitor(lv_task_t *param);

#endif
#ifdef DEV_BUILD
#include "dev_utils.h"
#endif
/**
 * @brief  The entry point to the application.
 * @retval int
 */
static void clear_state(felt_t *state, int size) {
  int i;

  for (i = 0; i < size; i++) {
    state[i][0] = 0;
    state[i][1] = 0;
    state[i][2] = 0;
    state[i][3] = 0;
  }
}

void print_state(felt_t *state, int size) {
  int i;

  for (i = 0; i < size; i++) {
    printf("%016" PRIx64, state[i][3]);
    printf("%016" PRIx64, state[i][2]);
    printf("%016" PRIx64, state[i][1]);
    printf("%016" PRIx64, state[i][0]);
    printf("\n");
  }
}
int main(void) {
#ifdef DEV_BUILD
  ekp_queue_init();
#endif
  application_init();

#ifdef RUN_ENGINE
#if USE_SIMULATOR == 0
  if (fault_in_prev_boot()) {
    handle_fault_in_prev_boot();
  } else
#endif /* USE_SIMULATOR == 0 */
  {
    logo_scr_init(2000);
    device_provision_check();
  }

  // char *mnemonic = "second tone shoe employ unfold lock donor uncle twice "
  //                  "nature ready fabric inspire lift language kangaroo leave
  //                  " "carry plug wild network hollow awake slab";
  // uint8_t seed[64];
  // // mnemonic_to_seed(mnemonic, NULL, seed, NULL);

  // uint32_t path[] = {0x80000000 + 0xA55,
  //                    0x80000000 + 0x4741E9C9,
  //                    0x80000000 + 0x447A6028,
  //                    0x80000000,
  //                    0x80000000,
  //                    0xC};

  // uint8_t pubkey[32], deployer[32], classhash[32], addr[32];
  // starknet_init();

  // hex_string_to_byte_array(
  //     "0229e9b0a11e54e5779cb336c113bbaa7a8f8adda36fc45ddca0732ec478dbfd",
  //     64,
  //     deployer);

  // mpz_t result;
  // mpz_init(result);
  // starknet_resource_bounds_t bounds = {
  //     .level_2.max_amount = {0x00, 0x00, 0x07},
  //     .level_2.max_price_per_unit = {0x00, 0x00, 0x03},
  //     .level_1.max_amount = {0x00, 0x00, 0x07},
  //     .level_1.max_price_per_unit = {0x00, 0x00, 0x03}};
  // felt_t felt = {0};
  // printf("\n");

  // felt_t transaction_hash_prefix = {0}, hash = {32123, 3213123, 321312,
  // 32131}; uint8_t hex[32] = {0}, sig[32] = {0}, stark_key[32] = {0};
  // hex_string_to_byte_array("696e766f6b65", 12, hex);
  // hex_to_felt_t(hex, 6, transaction_hash_prefix);

  starknet_sign_txn_unsigned_txn_t txn = {0};
  // hex_string_to_byte_array(
  //     "0229e9b0a11e54e5779cb336c113bbaa7a8f8adda36fc45ddca0732ec478dbfd",
  //     64,
  //     txn.sender_address);

  // hex_string_to_byte_array(
  //     "057260b0928dc7e15ada8911af893ca7e93b0ed14a3786288c73a6572f75214f",
  //     64,
  //     stark_key);
  // memcpy(&txn.resource_bound, &bounds, sizeof(bounds));
  // txn.version[0] = 0x03;
  // txn.calldata.value_count = 2;
  // txn.calldata.value[0].bytes[0] = 0;
  // txn.calldata.value[0].size = 1;
  // txn.calldata.value[1].bytes[0] = 0;
  // txn.calldata.value[1].bytes[1] = 1;
  // txn.calldata.value[1].bytes[2] = 2;
  // txn.calldata.value[1].bytes[3] = 3;
  // txn.calldata.value[1].bytes[4] = 4;
  // txn.calldata.value[1].bytes[5] = 5;

  // txn.calldata.value[1].size = 6;
  //   txn.calldata.value->bytes[1] = 0;
  // txn.calldata.value->size = 1;
  // starknet_compiled_call_data_t call_data;
  // starknet_init();
  // // calculate_invoke_transaction_hash(&txn, hash);
  // uint8_t buf[32] = {0};
  // felt_t_to_hex(hash, buf);
  // starknet_sign_digest(starkCurve, stark_key, buf, sig);

  // print_state(hash, 1);
  // printf("\n");
  // for (int i = 0; i < 64; i++) {
  //   printf("%02x", sig[i]);
  //   if (i == 31)
  //     printf("\n");
  // }
  // printf("\n");

  // fflush(stdout);

  while (1) {
    engine_ctx_t *main_engine_ctx = get_core_flow_ctx();
    engine_run(main_engine_ctx);
  }
#else /* RUN_ENGINE */
  while (true) {
    proof_of_work_task();

#if USE_SIMULATOR == 1
    usbsim_continue_loop();

#ifdef SDL_APPLE
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
#if USE_MOUSE != 0
      mouse_handler(&event);
#endif

#if USE_KEYBOARD
      keyboard_handler(&event);
#endif

#if USE_MOUSEWHEEL != 0
      mousewheel_handler(&event);
#endif
    }
#endif /*SDL_APPLE */
  }
#endif /* USE_SIMULATOR == 1 */

#endif /* RUN_ENGINE */
  return 0;
}

#if USE_SIMULATOR == 0

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state
   */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

/**
 * @brief Function to transmit data in real-time over SWV channel
 * @param file unused
 * @param *ptr string pointer for data to send
 * @param len  length of data to send
 *
 * @ret len of data transmitted
 */
int _write(int file, char *ptr, int len) {
#ifndef NDEBUG    // Disable printf in release mode
  int DataIdx;
  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    ITM_SendChar(*ptr++);
  }
  return len;
#endif
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
     file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

#endif    // USE_SIMULATOR
