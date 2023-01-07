/**
 * @file    write_to_cards_controller.c
 * @author  Cypherock X1 Team
 * @brief   Write to cards controller.
 *          This file contains the functions to write wallet to cards sequentially.
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
#include "constant_texts.h"
#include "controller_main.h"
#include "controller_tap_cards.h"
#include "flash_api.h"
#include "nfc.h"
#include "stdint.h"
#include "tasks.h"
#include "ui_instruction.h"
#include "ui_message.h"
#include "wallet.h"
#include "controller_tap_cards.h"
#include "shamir_wrapper.h"
#include "card_action_controllers.h"

extern Wallet_shamir_data wallet_shamir_data;

static uint32_t wallet_index; // What's the index of the wallet in flash to which we are talking to

static void write_share_to_card(uint8_t card_number);

void tap_cards_for_write_and_verify_flow_controller()
{
    switch (flow_level.level_four) {
    case CARD_ONE_FRONTEND:
        tap_card_data.tapped_card = 0;
        tap_card_data.desktop_control = false;
        flow_level.level_four = CARD_ONE_WRITE;
        break;

    case CARD_ONE_WRITE:
        write_share_to_card(1);
        break;

    case CARD_ONE_READBACK:
        readback_share_from_card(0);
        break;

    case CARD_TWO_FRONTEND:
        flow_level.level_four = CARD_TWO_WRITE;
        break;

    case CARD_TWO_WRITE:
        write_share_to_card(2);
        break;

    case CARD_TWO_READBACK:
        readback_share_from_card(1);
        break;

    case CARD_THREE_FRONTEND:
        flow_level.level_four = CARD_THREE_WRITE;
        break;

    case CARD_THREE_WRITE:
        write_share_to_card(3);
        break;

    case CARD_THREE_READBACK:
        readback_share_from_card(2);
        break;

    case CARD_FOUR_FRONTEND:
        flow_level.level_four = CARD_FOUR_WRITE;
        break;

    case CARD_FOUR_WRITE:
        write_share_to_card(4);
        break;

    case CARD_FOUR_READBACK:
        readback_share_from_card(3);
        break;

    default:
        reset_flow_level();
        message_scr_init(ui_text_something_went_wrong);
        break;
    }
}

static void write_share_to_card(uint8_t card_number)
{
    Flash_Wallet *wallet_for_flash = get_flash_wallet();
    wallet.xcor = card_number;
    tap_card_data.retries = 5;
    tap_card_data.lvl3_retry_point = flow_level.level_three;
    tap_card_data.lvl4_retry_point = flow_level.level_four - 1;
    memcpy(tap_card_data.family_id, get_family_id(), FAMILY_ID_SIZE);

    if (WALLET_IS_ARBITRARY_DATA(wallet.wallet_info))
        memcpy(wallet.arbitrary_data_share, ((uint8_t *) wallet_shamir_data.arbitrary_data_shares) + (card_number - 1) * wallet.arbitrary_data_size, wallet.arbitrary_data_size);
    else
        memcpy(wallet.wallet_share_with_mac_and_nonce, wallet_shamir_data.mnemonic_shares[card_number - 1], BLOCK_SIZE);
    memcpy(wallet.wallet_share_with_mac_and_nonce + BLOCK_SIZE, wallet_shamir_data.share_encryption_data[card_number - 1], NONCE_SIZE + WALLET_MAC_SIZE);

    while (1) {
        tap_card_data.acceptable_cards = 1 << (card_number - 1);
        if (card_number == 1) tap_card_data.tapped_card = 0;
        if (!tap_card_applet_connection())
            return;
        tap_card_data.status = nfc_add_wallet(&wallet);

        if (tap_card_data.status == SW_NO_ERROR) { // wallet added
            wallet_for_flash->cards_states = (1 << card_number) - 1;
            flow_level.level_four++;
            if (card_number == 1) {
                wallet_for_flash->state = UNVERIFIED_VALID_WALLET;
                add_wallet_to_flash(wallet_for_flash, &wallet_index);
            } else {
                put_wallet_flash(wallet_index, wallet_for_flash);
            }
            if (WALLET_IS_ARBITRARY_DATA(wallet.wallet_info))
                memset(((uint8_t *) wallet_shamir_data.arbitrary_data_shares) + (card_number - 1) * wallet.arbitrary_data_size, 0, wallet.arbitrary_data_size);
            else
                memset(wallet_shamir_data.mnemonic_shares[card_number - 1], 0, BLOCK_SIZE);
            memset(wallet_shamir_data.share_encryption_data[card_number - 1], 0, sizeof(wallet_shamir_data.share_encryption_data[card_number - 1]));
            memset(((uint8_t *) wallet_shamir_data.arbitrary_data_shares) + (card_number - 1) * wallet.arbitrary_data_size, 0, wallet.arbitrary_data_size);
            break;
        } else if (tap_card_handle_applet_errors()) {
            break;
        }
    }

    if (flow_level.show_error_screen && counter.level == LEVEL_ONE) {
        // if the error terminates the operation, clear any sensitive data
        memzero(wallet_shamir_data.share_encryption_data, sizeof(wallet_shamir_data.share_encryption_data));
        memzero(wallet_shamir_data.arbitrary_data_shares, sizeof(wallet_shamir_data.arbitrary_data_shares));
        if (card_number > 1) {
            flow_level.level_three = GENERATE_WALLET_FAILED_MESSAGE;
            flow_level.level_two = LEVEL_THREE_GENERATE_WALLET;
            flow_level.level_one = LEVEL_TWO_NEW_WALLET;
            counter.level = LEVEL_THREE;
        }
    }
}
