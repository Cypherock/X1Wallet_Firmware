/**
 * @file    add_coin_controller_b.c
 * @author  Cypherock X1 Team
 * @brief   Add coin back controller.
 *          Handles post event (only back/cancel events) operations for add coin flow initiated by desktop app.
 * @copyright Copyright (c) 2022 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/" target=_blank>https://mitcc.org/</a>
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
#include "communication.h"
#include "controller_level_four.h"

extern Wallet_credential_data wallet_credential_data;

void add_coin_controller_b() {
  switch (flow_level.level_three) {
    case ADD_COINS_VERIFY: {
      comm_reject_request(ADD_COIN_VERIFIED_BY_USER, 0);
      reset_flow_level();
      counter.next_event_flag = true;
    } break;

    case ADD_COINS_ENTER_PIN: {
      comm_reject_request(USER_REJECT_PIN_INPUT, 0);
      reset_flow_level();
      memzero(flow_level.screen_input.input_text,
              sizeof(flow_level.screen_input.input_text));
      counter.next_event_flag = true;
    } break;

    case ADD_COINS_ENTER_PASSPHRASE: {
      comm_reject_request(USER_REJECTED_PASSPHRASE_INPUT, 0);
      reset_flow_level();
      memzero(flow_level.screen_input.input_text,
              sizeof(flow_level.screen_input.input_text));
      counter.next_event_flag = true;
    } break;

    case ADD_COINS_CONFIRM_PASSPHRASE: {
      memzero(wallet_credential_data.passphrase,
              sizeof(wallet_credential_data.passphrase));
      flow_level.level_three = ADD_COINS_ENTER_PASSPHRASE;
    } break;

    default:
      break;
  }
}
