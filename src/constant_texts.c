/**
 * @file    constant_texts.c
 * @author  Cypherock X1 Team
 * @brief   Constant texts.
 *          Stores definitions of all the hardcoded user facing strings/texts.
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
#include "ui_menu.h"
#include "ui_text_slideshow.h"

// Main menu text
const char *ui_text_heading_main_menu = "Main Menu";
const char *ui_text_options_main_menu[] = {"Create Wallet",
                                           //"Add arbitrary data",
                                           "Settings"};

// level two
const char *ui_text_options_old_wallet[] = {"View Seed", "Delete Wallet"};

// New wallet menu text
const char *ui_text_heading_new_wallet = "Create Wallet";
const char *ui_text_options_new_wallet[NUMBER_OF_OPTIONS_NEW_WALLET] = {
    "Generate New Wallet",
    "Restore from Seed"};

const char *ui_text_mnemonics_number_options[NUMBER_OF_OPTIONS_MNEMONIC_INPUT] =
    {"12", "18", "24"};

// Settings menu text
const char *ui_text_heading_settings = "Settings";
const char *ui_text_options_settings[NUMBER_OF_OPTIONS_SETTINGS] = {
    "Restore Wallets from Cards",
    "Check Card Health",
    "Rotate Display",
    "Toggle Log export",
    "Toggle Passphrase Step",
    "Clear Device Data",
    "Factory Reset",
    "Device Info",
    "View Card Version",
    "Regulatory Info",
    "Pair Cards",
#ifdef DEV_BUILD
    "Buzzer toggle",
#endif
};

const char *ui_text_rotate_display_confirm =
    "Display will rotate 180 degrees\nConfirm?";

const char *ui_text_options_passphrase[] = {
    "Disable Passphrase Step",
    "Enable Passphrase Step",
};

const char *ui_text_options_logging_export[] = {
    "Disable Logs",
    "Enable Logs",
};

const char *ui_text_pair_card_confirm =
    "Do you want to proceed with card pairing?";
const char *ui_text_card_pairing_success = "All cards are paired";
const char *ui_text_card_pairing_warning = "Pair all cards from Settings";

#if DEV_BUILD
const char *ui_text_options_buzzer_adjust[] = {
    "Toggle buzzer",
    "Disable",
    "Enable",
};
#endif /* DEV_BUILD */

// Regulatory info text
const char *ui_text_regulatory_info[NUMBER_OF_SLIDES_REGULATORY_INFO] = {
    "This device complies with Part 15 of the FCC Rules. Operation is",
    "subject to the following two conditions: (1) this device may not",
    "cause harmful interference, and (2) this device must accept any",
    "interference received, including interference that",
    "may cause undesired operation",
};

// Factory reset text
const char *ui_text_factory_reset_instruction =
    "You will need 2 cards to perform factory reset\nMake sure at least 2 "
    "cards are accessible\nDo you want to proceed?";
const char *ui_text_confirm_factory_reset =
    "This will erase all wallets from device\nYour device will reset to "
    "factory default settings.\nConfirm?";
const char *ui_text_erasing = "Factory reset done Device will restart";
const char *ui_text_reset_exit[NUMBER_OF_SLIDES_FACTORY_RESET_EXIT] = {
    "Exiting without factory reset",
    "Press the center button to proceed",
};

// Card health check text
const char *ui_text_card_seems_healthy = "Card seems healthy";
const char *ui_text_click_to_view_wallets = "Click Ok to view wallets";
const char *ui_text_no_wallets_fetched =
    "No Wallets fetched\nClick Ok to return";
const char *ui_text_card_health_check_start = "Tap a card to check it's health";
const char *ui_text_card_health_check_error[] = {
    "Card error: ",
    "Visit cypherock.com/error-codes for more info"};

// Sync wallets with cards text
const char *ui_text_syncing_complete = "wallet(s) synced";

// Clear device data text
const char *ui_text_clear_device_data_instruction =
    "You will need 2 cards to perform clear device data\nMake sure at least 2 "
    "cards are accessible\nDo you want to proceed?";
const char *ui_text_confirm_clear_device_data =
    "This will erase all wallets from device\nYour device will reset and go to "
    "main menu.\nConfirm?";
const char *ui_text_erasing_device_data =
    "Device data cleared Device will restart";
const char
    *ui_text_clear_wallet_data_exit[NUMBER_OF_SLIDES_FACTORY_RESET_EXIT] = {
        "Exiting without clearing device data",
        "Press the center button to proceed",
};

// Manager app text
// Device authentication text
const char *ui_text_message_device_authenticating = "Device\nauthenticating...";
const char *ui_text_message_device_auth_success =
    "Device authenticated successfully";
const char *ui_text_message_device_auth_failure =
    "Device authentication failed";

const char *ui_text_joystick_up = "Toggle joystick up";
const char *ui_text_joystick_right = "Toggle joystick right";
const char *ui_text_joystick_down = "Toggle joystick down";
const char *ui_text_joystick_left = "Toggle joystick left";
const char *ui_text_joystick_center = "Center click joystick";
const char *ui_text_joystick_checkup_complete =
    "Joystick instructions complete";
const char *ui_text_wallet_selector_invalid =
    "The selected wallet is misconfigured\n"
    "Visit the wallet from main menu to fix it\n"
    "Or choose a different wallet to continue";

// Onboarding flow text
const char *ui_text_onboarding_welcome = "Welcome";
const char *ui_text_onboarding[NUMBER_OF_SLIDESHOW_SCREENS_ONBOARDING] = {
    "Visit\ncypherock.com/go",
    "And download the cySync app to set up the device",
    "Follow instructions on the cySync app"};
const char *ui_text_onboarding_complete =
    "Congratulations!\nCypherock X1 is ready to use";

// wallet creation
const char *ui_text_verification_is_now_complete_messages[] = {
    "Wallet created successfully!",
    "You can view the seed phrase on the device",
    "You'll need this device and a card to make transactions",
    "You'll require the PIN to view seed or transact",
    "To get started export this wallet to the cySync app",
    "Press the center button to return to the main menu",
};

const char *ui_text_seed_phrase_will_be_shown_copy_to_verify[] = {
    "Write down the seed phrase on paper to verify",
    "Press the center button to view seed phrase"};

const char *ui_text_startup_instruction_screen_1[] = {
    "Press the button on device to start"};

const char *ui_text_startup_instruction_screen_2[] = {
    "This device provides 5 way joystick for navigation",
    "Toggle left or right to change options horizontally",
    "Toggle up or down to change options vertically",
    "Use centre click to select highlighted option",
    "If you are ready use centre click to proceed"};

const char *ui_text_startup_instruction_screen_4[] = {
    "Now visit cypherock.com/gs",
    "and download the cySync app to set up the device"

};

const char *ui_text_view_data = "View data";

// product id
const char *product_hash = "0x2ea340d9";

// errors
const char *ui_text_something_went_wrong = "Something went wrong! Retry";
const char *ui_text_something_went_wrong_contact_support_send_logs =
    "Something went wrong! Contact support";
const char *ui_text_already_have_maxi_wallets =
    "You already have maximum number of wallets";
const char *ui_text_aborted = "Aborted from\nthe cySync app";
const char *ui_text_operation_has_been_cancelled =
    "Operation has been cancelled!";
const char *ui_text_wallet_name_exists =
    "Name already exists Pick a different name";
const char *ui_text_pin_incorrect_re_enter = "Wrong PIN!\nEnter correct pin";
const char *ui_text_wallet_with_same_mnemo_exists =
    "Wallet already present on device";
const char *ui_text_incorrect_mnemonics = "Incorrect mnemonics";
const char *ui_text_incorrect_choice_view_seed_again =
    "Incorrect choice!\nView the seed phrase again?";
const char *ui_text_xpub_not_found_add_coin_first =
    "Click OK on the cySync app to resync coins";
const char *ui_wallet_pin_instruction_2 =
    "If you forget it\nyou will lose access\nto this wallet";
const char *ui_wallet_passphrase_instruction_2 =
    "If you forget it\nyou will lose access\nto this wallet";
const char *ui_wallet_passphrase_instruction_3 =
    "Passphrase is recommended only for advanced users";
const char *ui_wallet_passphrase_instruction_4 =
    "You will be entering it each time you transact with this wallet";
const char *ui_text_invalid_transaction = "Invalid transaction";

// errors X1 Card flows
const char *ui_text_authentication_required = "Device authentication required";
const char *ui_text_start_auth_from_CySync =
    "Authenticate device from the cySync app";
const char *ui_text_invalid_card_contact_cypherock =
    "Authentication failed Contact Cypherock";
const char *ui_text_wrong_card_sequence = "Wrong card sequence";
const char *ui_text_tap_another_card = "Tap another card";
const char *ui_text_wallet_doesnt_exists_on_this_card =
    "Wallet does not exist on this card";
const char *ui_text_wallet_verification_failed_in_creation =
    "Wallet not created Proceed for deletion";
const char *ui_text_wallet_verification_failed_in_reconstruction =
    "Verification failed.\n Contact support.";
const char *ui_text_no_response_from_desktop =
    "No response from the cySync app!\nTry again";

const char *ui_text_tap_a_card = "Tap any card";

const char *ui_text_invalid_card_tap_card[] = {
    "Invalid card\nTap Card #1",
    "Invalid card\nTap Card #2",
    "Invalid card\nTap Card #3",
    "Invalid card\nTap Card #4",
};

const char *ui_text_device_authenticating[] = {"Device authenticating.",
                                               "Device authenticating..",
                                               "Device authenticating...",
                                               "Device authenticating....",
                                               "Device authenticating....."};

const char *ui_text_device_provisioning[] = {"Device provisioning.",
                                             "Device provisioning..",
                                             "Device provisioning...",
                                             "Device provisioning....",
                                             "Device provisioning....."};

const char *ui_text_tap_a_card_instruction1 =
    "This device interacts with cards using NFC";
const char *ui_text_tap_a_card_instruction2 =
    "Place and hold any card below the device to test NFC";

const char *ui_text_tap_1_2_cards = "Tap any card";
const char *ui_text_tap_2_2_cards = "Tap another card";
const char *ui_text_place_card_below = "Place card\nbelow the device";

const char *ui_text_wallet_id_collision = "Wallet already exists";
const char *ui_text_card_is_full = "Error!\nCard is full";

const char *ui_text_device_synced_with_card =
    "Device synced successfully with the card";
const char *ui_text_family_id_retrived = "Family ID retrieved";
const char *ui_text_family_id_mismatch =
    "Error! Use correct cards of the wallet";

const char *ui_text_wrong_wallet_is_now_locked =
    "Wrong PIN!\nWallet is now locked";
const char *ui_text_wallet_already_unlocked = "Wallet already unlocked";
const char *ui_text_hash_not_accepted_by_card = "Hash not accepted by card!";

const char *ui_text_do_not_detach_device =
    "Keep the device connected throughout the process";

const char *ui_text_card_command_send_error =
    "Error occurred while sending to APDU to card";
const char *ui_text_card_update_done = "Card update done!";
const char *ui_text_card_transaction_exception =
    "Operation failed on card (Tx Exp)";
const char *ui_text_card_out_of_boundary_exception =
    "Operation failed on card (OOB)";
const char *ui_text_card_null_pointer_exception =
    "Error! Invalid memory accessed";
const char *ui_text_card_crypto_exception =
    "Operation failed on card (Crypto Exp)";
const char *ui_text_card_invalid_apdu_length =
    "Wallet with same name or seed already exists";
const char *ui_text_card_invalid_tag_in_apdu =
    "Operation failed on card (Tag exp)";
const char *ui_text_unknown_error_contact_support =
    "Unknown card error! Contact support";
const char *ui_text_unauthenticated_device = "Device is unauthenticated";

// headings
const char *ui_text_enter_pin = "Enter PIN";
const char *ui_text_confirm_pin = "Confirm PIN";
const char *ui_text_word_hash = "Word # ";
const char *ui_text_verify_word_hash = "Verify Word # ";
const char *ui_text_enter_wallet_name = "Enter wallet name";
const char *ui_text_number_of_words = "Number of words";
const char *ui_text_add_coins = "Add Coin #";
const char *ui_text_resync_coins = "Resync Coin #";
const char *ui_text_choose_wallet = "Choose Wallet";
const char *ui_text_enter_passphrase = "Enter passphrase";
const char *ui_text_confirm_passphrase = "Confirm passphrase";
const char *ui_text_receive_on = "Receive on";
const char *ui_text_verify_address = "Verify address";
const char *ui_text_verify_amount = "Verify amount";
const char *ui_text_verify_contract = "Verify contract";
const char *ui_text_unverified_contract =
    LV_SYMBOL_WARNING " Warning!\nUnverified contract";
const char *ui_text_confirm_wallet_name = "Confirm wallet name";
const char *ui_text_enter_data = "Enter data";
const char *ui_text_confirm_data = "Confirm data";
const char *ui_text_verification_cancelled = "Verification cancelled";
const char *ui_text_verify_nonce = "Verify nonce";
const char *ui_text_waiting_for_desktop =
    "Follow the\ninstructions on\nthe cySync app";

// headings near specific
const char *ui_text_verify_create_from = "Verify Create from";
const char *ui_text_verify_new_account_id = "Verify New Account Id";
const char *ui_text_new_account_id = "new_account_id";
const char *ui_text_new_public_key = "new_public_key";
const char *ui_text_replace_account = "Replace Account";
const char *ui_text_confirm_account = "Confirm Account";
const char *ui_text_near_transfer_action_type = "transfer";
const char *ui_text_near_create_account_method = "create_account";

// headings X1 Card flow
const char *ui_text_family_id_hex = "F. Id (Hex)";

// messages
const char *ui_text_use_passphrase_question =
    "Do you want to use passphrase with this wallet?";
const char *ui_text_wallet_name_size_limit =
    "Wallet name cannot exceed 15 letters";
const char *ui_text_check_cysync = "Check the cySync app";
const char *ui_text_tap_x1card_auth_instruction_1 =
    "Tap a card and do not lift until you hear 2 beep sound";
const char *ui_text_tap_x1card_auth_instruction_2 =
    "Now tap the same card again";
const char *ui_text_tap_card_to_start = "Tap a card to start";
const char *ui_text_wallet_deleted_successfully =
    "Wallet deleted successfully!";
const char *ui_text_recovery_successfull = "Recovery Successful!";
const char *ui_text_processing = "Processing...";
const char *ui_text_press_enter_to_start_verification =
    "Proceed to verify wallet on cards";
const char *ui_text_now_enter_your_seed_phrase = "Now enter your\nseed phrase";
const char *ui_text_now_enter_your_data = "Now enter your\ndata";
const char *ui_text_verify_entered_words = "Verify the entered words";
const char *ui_text_exported_signed_transaction_to_desktop =
    "Exported signed transaction to the cySync app!";
const char *ui_text_start_card_update =
    "Start card update?\nThis will format the card";
const char *ui_text_start_verification_of_card = "Start verification of card";
const char *ui_text_wallet_lock_continue_to_unlock =
    "Wallet is locked Proceed to unlock";
const char *ui_text_wallet_partial_continue_to_delete =
    "Wallet in partial\nstate, proceed to\ndelete";
const char *ui_text_wallet_not_verified_continue_to_verify =
    "Wallet not verified Proceed to verify";
const char *ui_text_wallet_out_of_sync_continue_to_sync_with_x1cards =
    "Proceed to Sync\nwith cards";
const char *ui_text_wallet_visit_to_verify = "Visit the wallet to fix it";
const char *ui_text_wallet_partial_fix =
    "Wallets in partial state Fix before proceeding";
#ifdef ALLOW_LOG_EXPORT
const char *ui_text_sending_logs = "Sending logs...";
const char *ui_text_logs_sent = "Logs sent";
#endif
const char *ui_text_creation_failed_delete_wallet =
    "Wallet creation failed\nProceed to delete";

const char *ui_text_successfull = "Successful";
const char *ui_text_view_seed_messages =
    "Do not give the seed phrase to anyone.\nYou will lose your coins if the "
    "seed phrase gets stolen.\nDo you want to proceed?";
const char *ui_text_card_authentication_success =
    "Card authentication successful";
const char *ui_text_card_authentication_failed = "Card authentication failed";
const char *ui_text_enable_passphrase_step =
    "Do you want to enable passphrase\n step on wallet creation?";
const char *ui_text_disable_passphrase_step =
    "Do you want to disable passphrase\n step on wallet creation?";
const char *ui_text_warning_txn_fee_too_high =
    "WARNING!\nTransaction fees\ntoo high, proceed?";
const char *ui_text_enable_log_export = "Do you want to enable logging?";
const char *ui_text_disable_log_export = "Do you want to disable logging?";

//?
const char *ui_text_do_you_want_import_wallet_to_desktop =
    "Do you want to import wallet to the cySync app?";
const char *ui_text_do_you_want_to_set_pin = "Do you want to set a PIN?";

const char *ui_text_auth_process = "Waiting for device authentication...";

const char *ui_text_sync_x1card_confirm =
    "Device will be synced with wallets in card Confirm?";

const char *ui_text_no_wallets_present = "No Wallets Present in card";
const char *ui_text_device_verification_success =
    "Device successfully authenticated";
const char *ui_text_device_verification_failure =
    "Device authentication failed";
const char *ui_text_unauthenticate_via_cysync =
    "Authenticate the device via the cySync app";
const char *ui_text_start_device_verification = "Start device authentication?";
const char *ui_text_its_a_while_check_your_cards =
    "It's been a while check if your cards are working properly";
const char *ui_text_delete_this_wallet = "Delete this wallet";
const char *ui_text_need_all_x1cards_to_create_wallet =
    "You'll need all cards to create a wallet\nDo you want to "
    "proceed?";
const char *ui_text_need_all_x1cards_to_delete_wallet_entirely =
    "You'll need all cards to delete this wallet entirely, do you want to "
    "proceed?";
const char *ui_text_20_spaces = "                    ";
const char *ui_text_wallet_not_found_on_x1card = "No Wallet found on card";
const char *ui_text_sync_wallets_next_steps = "Sync each wallet in next steps";
const char *ui_text_corrupted_card_contact_support =
    "Corrupted card! Contact support";
const char *ui_text_incompatible_card_version =
    "Incompatible card version! Contact support";
const char *ui_text_card_error_contact_support =
    "Contact support & share the device logs";
const char *ui_text_device_and_card_not_paired =
    "This device & card are not paired";
const char *ui_text_security_conditions_not_met =
    "Security conditions not met!";
const char *ui_text_retry_or_repair = "Retry or re-pair your cards";
const char *ui_text_process_reset_due_to_inactivity =
    "Operation aborted due to inactivity";
const char *ui_text_device_already_provisioned = "Device already provisioned";
const char *ui_text_check_cysync_app = "Check the cySync app";
const char *ui_text_provision_success = "Device provision successful";
const char *ui_text_provision_fail = "Device provision unsuccessful";
const char *ui_text_cannot_verify_card_contact_support =
    "Couldn't verify card! Contact support";
const char *ui_text_wrong_btc_transaction = "Wrong transaction formatting";
const char *ui_text_btc_change_address_mismatch = "Change address mismatch";
const char *ui_text_worng_eth_transaction = "Invalid transaction detected";
const char *ui_text_error_no_card_paired = "No cards paired with device";
const char *ui_text_error_pair_all_cards = "Pair all four cards from Settings";
const char *ui_text_error_pair_atleast_2_cards =
    "Pair at least two cards from Settings";
const char *ui_text_continue_with_pairing = "Proceed with pairing?";
const char *ui_text_device_compromised =
    "Device is compromised! Contact support";
const char *ui_text_device_compromised_not_provisioned =
    "Device is not provisioned";
const char *ui_text_device_compromised_partially_provisioned =
    "Device partially provisioned";
const char *ui_text_device_compromised_v1_config =
    "Device provisioned with v1 config";
const char *ui_text_pow_challenge_failed = "Wallet unlock failed! Retry";
const char *ui_text_card_removed_fast =
    "Too fast, tap again and wait for buzzer sound";
const char *ui_text_card_freq_discon_fault =
    "Frequent disconnections detected!\nContact support";
const char *ui_text_card_align_with_device_screen =
    "Hold the card and align with the device screen";
const char *ui_text_remove_card_prompt = "Process complete Remove card";
const char *ui_text_card_detected = "Card detected Processing...";
const char *ui_text_nfc_hardware_fault_detected =
    "NFC antenna fault detected! Contact Support";
const char *ui_text_unreliable_cards =
    "Warning, your cards are unreliable! Contact support";
const char *ui_critical_card_health_migrate_data =
    "Card health is critical! Migrate to new set of cards";

// App specific

// Inheritance
const char *ui_text_inheritance_wallet_auth_flow_confirmation =
    "Start %s wallet authentication?";
const char *ui_text_inheritance_wallet_auth_flow_confirmation_generic =
    "Start wallet authentication?";
const char *ui_text_inheritance_wallet_authenticating =
    "Wallet\nauthenticating...";
const char *ui_text_inheritance_wallet_auth_success =
    "Wallet\nauthenticated\nsuccessfully";
const char *ui_text_inheritance_wallet_auth_fail =
    "Wallet\nauthentication\nfailed";

const char *ui_text_inheritance_encryption_flow_confirmation =
    "Proceed to encrypt data for %s?";
const char *ui_text_inheritance_encryption_flow_success = "Encryption Success";
const char *ui_text_inheritance_encryption_flow_failure = "Encryption Failed";

const char *ui_text_inheritance_decryption_flow_confirmation =
    "Proceed to decrypt data for %s?";
const char *ui_text_inheritance_decryption_flow_confirmation_generic =
    "Proceed to decrypt data?";
const char *ui_text_inheritance_decryption_flow_success = "Decryption Success";
const char *ui_text_inheritance_decryption_flow_failure = "Decryption Failed";

// ICP

const char *ui_text_principal_id = "Principal Id";
const char *ui_text_account_id = "Account Id";

#ifdef ALLOW_LOG_EXPORT
const char *ui_text_send_logs_prompt = "Send logs to the cySync app?";
#endif
