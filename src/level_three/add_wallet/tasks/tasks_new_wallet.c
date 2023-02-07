/**
 * @file    tasks_new_wallet.c
 * @author  Cypherock X1 Team
 * @brief   New wallet task.
 *          This file contains the task that handles new wallet creation.
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
#include "bip39_english.h"
#include "constant_texts.h"
#include "tasks.h"
#include "tasks_add_wallet.h"
#include "ui_confirmation.h"
#include "ui_delay.h"
#include "ui_input_text.h"
#include "ui_instruction.h"
#include "ui_message.h"
#include "ui_multi_instruction.h"
#include "ui_address.h"
#include "tasks_tap_cards.h"
#include "ui_list.h"
#include "controller_main.h"

extern Flash_Wallet wallet_for_flash;

extern char* ALPHABET;
extern char* ALPHA_NUMERIC;
extern char* NUMBERS;

extern Wallet_credential_data wallet_credential_data;

void tasks_add_new_wallet()
{
    if (flow_level.show_error_screen) {
        message_scr_init(flow_level.error_screen_text);
        return;
    }

    switch (flow_level.level_three) {
    case GENERATE_WALLET_NAME_INPUT: {
        input_text_init(
            ALPHABET,
            ui_text_enter_wallet_name,
            2,
            DATA_TYPE_TEXT,
            15);
    } break;

    case GENERATE_WALLET_NAME_INPUT_CONFIRM: {
        char display[65];
        snprintf(display, sizeof(display), "%s", flow_level.screen_input.input_text);
        address_scr_init(ui_text_confirm_wallet_name, display, false);
    } break;

    case GENERATE_WALLET_PIN_INSTRUCTIONS_1: {
      char display[65];
      if(strnlen(flow_level.screen_input.input_text, sizeof(flow_level.screen_input.input_text)) <= 15)
        snprintf(display, sizeof(display), UI_TEXT_PIN_INS1, wallet.wallet_name);
      else
        snprintf(display, sizeof(display), UI_TEXT_PIN_INS1, "this wallet");
      delay_scr_init(display, DELAY_TIME);
    } break;

    case GENERATE_WALLET_PIN_INSTRUCTIONS_2: {
      
      delay_scr_init(ui_wallet_pin_instruction_2,DELAY_TIME);
    
    } break;

    case GENERATE_WALLET_SKIP_PIN: {
        confirm_scr_init(ui_text_do_you_want_to_set_pin);
        confirm_scr_focus_cancel();
    } break;

    case GENERATE_WALLET_PIN_INPUT: {
        input_text_init(
            ALPHA_NUMERIC,
            ui_text_enter_pin,
            4,
            DATA_TYPE_PIN,
            8);
    } break;

    case GENERATE_WALLET_PIN_CONFIRM: {
        input_text_init(
            ALPHA_NUMERIC,
            ui_text_confirm_pin,
            4,
            DATA_TYPE_PIN,
            8);
    } break;

    case GENERATE_WALLET_PASSPHRASE_INSTRUCTIONS_1: {
      char display[65];
      if(strnlen(flow_level.screen_input.input_text, sizeof(flow_level.screen_input.input_text)) <= 15)
        snprintf(display, sizeof(display), UI_TEXT_PASSPHRASE_INS1, wallet.wallet_name);
      else
        snprintf(display, sizeof(display), UI_TEXT_PASSPHRASE_INS1, "this wallet");
      delay_scr_init(display, DELAY_TIME);
    } break;

    case GENERATE_WALLET_PASSPHRASE_INSTRUCTIONS_2: {
      delay_scr_init(ui_wallet_passphrase_instruction_2, DELAY_TIME);
    } break;

    case GENERATE_WALLET_PASSPHRASE_INSTRUCTIONS_3: {
      delay_scr_init(ui_wallet_passphrase_instruction_3, DELAY_TIME);
    } break;

    case GENERATE_WALLET_PASSPHRASE_INSTRUCTIONS_4: {
      delay_scr_init(ui_wallet_passphrase_instruction_4, DELAY_TIME);
    } break;

    case GENERATE_WALLET_SKIP_PASSPHRASE: {
        confirm_scr_init(ui_text_use_passphrase_question);
        confirm_scr_focus_cancel();
    } break;

    case GENERATE_WALLET_PROCESSING: {
        instruction_scr_init(ui_text_processing, NULL);
        mark_event_over();
    } break;

    case GENERATE_WALLET_SEED_GENERATE: {
        instruction_scr_destructor();
        mark_event_over();
    } break;

    case GENERATE_WALLET_SEED_GENERATED: {
        message_scr_init(ui_text_seed_generated_successfully);
    } break;

    case GENERATE_WALLET_SAVE_WALLET_SHARE_TO_DEVICE:
        mark_event_over();
        break;

    case GENERATE_WALLET_TAP_CARD_FLOW: {
        tap_cards_for_write_flow();
    } break;

    case GENERATE_WALLET_VERIFY_SHARES:
        instruction_scr_init(ui_text_processing, "");
        instruction_scr_change_text(ui_text_processing, true);
        BSP_DelayMs(DELAY_SHORT);
        mark_event_over();
        break;

    case GENERATE_WALLET_SUCCESS_MESSAGE: {
        instruction_scr_destructor();
        const char *messages[6] = {
            ui_text_verification_is_now_complete_messages[0], ui_text_verification_is_now_complete_messages[1],
            ui_text_verification_is_now_complete_messages[2], ui_text_verification_is_now_complete_messages[4],
            ui_text_verification_is_now_complete_messages[5], NULL};
        uint8_t count = 5;

        if (WALLET_IS_PIN_SET(wallet.wallet_info)) {
            messages[3] = ui_text_verification_is_now_complete_messages[3];
            messages[4] = ui_text_verification_is_now_complete_messages[4];
            messages[5] = ui_text_verification_is_now_complete_messages[5];
            count = 6;
        }

        multi_instruction_init(messages, count, DELAY_LONG_STRING, true);
    } break;

    case GENERATE_WALLET_FAILED_MESSAGE:
        instruction_scr_destructor();
        message_scr_init(ui_text_creation_failed_delete_wallet);
        break;

    default:
        message_scr_init(ui_text_something_went_wrong);
        //TODO: Add reset flow level
        break;
    }
}
