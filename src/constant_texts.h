/**
 * @file    constant_texts.h
 * @author  Cypherock X1 Team
 * @brief   Header for constant texts.
 *          Stores declaration of all the hardcoded user facing strings/texts.
 * @copyright Copyright (c) 2022 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 * target=_blank>https://mitcc.org/</a>
 *
 */
#ifndef CONSTANT_TEXTS_H
#define CONSTANT_TEXTS_H

#include "ui_menu.h"
#include "ui_text_slideshow.h"

#define UI_TEXT_CARD_TAPPED "Card #%d Tapped"
#define UI_TEXT_TAP_CARD_TO_TEST                                               \
  "Place any card below the device to test card tapping"
#define UI_TEXT_PERMANENTLY_DELETE                                             \
  "This will permanently delete %s wallet\nfrom your device and all cards. "   \
  "Confirm?"
#define UI_TEXT_ENTER_WORD "Enter Word # %d"
#define UI_TEXT_IS_WORD "Is Word #%d\n%s"
#define UI_TEXT_TAP_CARD "Tap card #%d"
#define UI_TEXT_PLACE_CARD "Place Card #%d"
#define UI_TEXT_RECEIVE_PROMPT "Receive %s in %s"
#define UI_TEXT_ADD_ACCOUNT_PROMPT "Add %s to %s"
#define UI_TEXT_RECEIVE_TOKEN_PROMPT "Receive %s on %s in %s"
#define UI_TEXT_BTC_SEND_PROMPT "Send %s from %s"
#define UI_TEXT_SEND_PROMPT "Send %s on %s"
#define UI_TEXT_SIGN_TXN_PROMPT "Sign transaction on %s from %s"
#define UI_TEXT_REVIEW_TXN_PROMPT "Review transaction to %s"
#define UI_TEXT_SEND_TOKEN_PROMPT "Send %s on %s from %s"
#define UI_TEXT_BTC_RECEIVER "Receiver #%d"
#define UI_TEXT_BTC_FEE "Transaction fee"
#define UI_TEXT_SIGN_PROMPT "Sign %s message on %s from %s"
#define UI_TEXT_TXN_FEE "Transaction fee"
#define UI_TEXT_SEND_TXN_FEE "%s %s"
#define UI_TEXT_VERIFY_AMOUNT "Verify amount\n%s\n%s"
#define UI_TEXT_PAIRING_TAP_CARD "Tap Card #%d to pair"
#define UI_TEXT_WALLET_LOCKED_WAIT_MSG "%s is currently locked\nWait for %d %s"
#define UI_TEXT_PIN_INS1 "In next step you can setup an alphanumeric PIN for %s"
#define UI_TEXT_PASSPHRASE_INS1 "In next step you can setup Passphrase for %s"
#define UI_TEXT_PLACE_CARD_TILL_BEEP "Do not lift until you hear %d beep sound"
#define UI_TEXT_VERIFY_MESSAGE "Verify Message"
#define UI_TEXT_VERIFY_DOMAIN "Verify Domain"
#define UI_TEXT_EIP712_DOMAIN_TYPE "EIP712Domain"
#define UI_TEXT_WALLET_UNLOCKED "Correct PIN! %s is now unlocked"
#define UI_TEXT_UNLOCK_WARNING "Multiple incorrect attempts may block %s"
#define UI_TEXT_INCORRECT_PIN_ATTEMPTS_REMAINING                               \
  "Wrong PIN!\n%d attempt(s) left"
#define UI_TEXT_BLIND_SIGNING_WARNING                                          \
  LV_SYMBOL_WARNING " Blind Signing\nProceed at your own risk!"
#define UI_TEXT_VERIFY_HD_PATH "Verify Derivation Path"
#define UI_TEXT_PIN "PIN\n %s"
#define UI_TEXT_VERIFY_DESTINATION_TAG "Verify Destination Tag\n%lu"

// product hash
extern const char *product_hash;

// Main menu text
#define NUMBER_OF_OPTIONS_MAIN_MENU 2

#define MAIN_MENU_CREATE_WALLET_INDEX 0
#define MAIN_MENU_SETTINGS_INDEX 1
extern const char *ui_text_heading_main_menu;
extern const char *ui_text_options_main_menu[];

// Old wallet menu text
#define NUMBER_OF_OPTIONS_OLD_WALLET 2
extern const char *ui_text_options_old_wallet[];

// New wallet menu text
#define NUMBER_OF_OPTIONS_NEW_WALLET 2
extern const char *ui_text_heading_new_wallet;
extern const char *ui_text_options_new_wallet[];

#define NUMBER_OF_OPTIONS_MNEMONIC_INPUT 3
extern const char *ui_text_mnemonics_number_options[];

// Settings menu text
#ifdef DEV_BUILD
#define NUMBER_OF_OPTIONS_SETTINGS 12
// TODO: Update after refactor - remove the following MACRO
#define NUMBER_OF_OPTIONS_ADVANCED_OPTIONS NUMBER_OF_OPTIONS_SETTINGS
#else
#define NUMBER_OF_OPTIONS_SETTINGS 11
// TODO: Update after refactor - remove the following MACRO
#define NUMBER_OF_OPTIONS_ADVANCED_OPTIONS NUMBER_OF_OPTIONS_SETTINGS
#endif /* DEV_BUILD*/
extern const char *ui_text_heading_settings;
extern const char *ui_text_options_settings[NUMBER_OF_OPTIONS_SETTINGS];
extern const char *ui_text_rotate_display_confirm;
extern const char *ui_text_options_logging_export[];
extern const char *ui_text_options_passphrase[];

extern const char *ui_text_pair_card_confirm;
extern const char *ui_text_card_pairing_success;
extern const char *ui_text_card_pairing_warning;
#define PAIR_CARD_MESSAGE "%d card(s) paired successfully"

#if DEV_BUILD
extern const char *ui_text_options_buzzer_adjust[];
#endif /* DEV_BUILD */

// Regulatory info text
#define NUMBER_OF_SLIDES_REGULATORY_INFO 5
extern const char *ui_text_regulatory_info[];

// Version info text
#define UI_TEXT_CARD_VERSION "Card Version\n%d.%d.%d-%s"
#define UI_TEXT_FIRMWARE_VERSION "Firmware Version\n%d.%d.%d-%s"
#define UI_TEXT_BOOTLOADER_VERSION "Bootloader Version\n%d.%d.%d"

// Factory reset text
extern const char *ui_text_factory_reset_instruction;
extern const char *ui_text_confirm_factory_reset;
extern const char *ui_text_erasing;
#define NUMBER_OF_SLIDES_FACTORY_RESET_EXIT 2
extern const char *ui_text_reset_exit[NUMBER_OF_SLIDES_FACTORY_RESET_EXIT];
#define UI_TEXT_FACTORY_RESET_ERROR "Wallet '%s' not found on card(s)"

// Card health check text
extern const char *ui_text_card_seems_healthy;
extern const char *ui_text_click_to_view_wallets;
extern const char *ui_text_no_wallets_fetched;
extern const char *ui_text_card_health_check_start;
extern const char *ui_text_card_health_check_error[];
#define UI_TEXT_CARD_HEALTH_CHECK_ERROR "Wallets in Card #%d"

// Sync wallets with cards text
#define UI_TEXT_SYNC_WALLET_PROMPT "Do you want to sync wallet %s?"
#define UI_TEXT_SYNC_WALLET_LOCKED "Wallet %s is locked"
#define UI_TEXT_SYNC_WALLET_DONE "Syncing %s complete"
extern const char *ui_text_syncing_complete;

// Clear user data text
extern const char *ui_text_clear_device_data_instruction;
extern const char *ui_text_confirm_clear_device_data;
extern const char *ui_text_erasing_device_data;
extern const char
    *ui_text_clear_wallet_data_exit[NUMBER_OF_SLIDES_FACTORY_RESET_EXIT];

// Manager app text
// Device authentication text
extern const char *ui_text_message_device_authenticating;
extern const char *ui_text_message_device_auth_success;
extern const char *ui_text_message_device_auth_failure;

extern const char *ui_text_joystick_up;
extern const char *ui_text_joystick_right;
extern const char *ui_text_joystick_down;
extern const char *ui_text_joystick_left;
extern const char *ui_text_joystick_center;
extern const char *ui_text_joystick_checkup_complete;
extern const char *ui_text_wallet_selector_invalid;

// Onboarding text
extern const char *ui_text_onboarding_welcome;
#define NUMBER_OF_SLIDESHOW_SCREENS_ONBOARDING 3
extern const char *ui_text_onboarding[];
extern const char *ui_text_onboarding_complete;

// Firmware update text
#define FIRMWARE_UPDATE_CONFIRMATION "Update firmware to version %d.%d.%d"

extern const char *ui_text_verification_is_now_complete_messages[];

extern const char *ui_text_seed_phrase_will_be_shown_copy_to_verify[];

extern const char *ui_text_startup_instruction_screen_1[];

extern const char *ui_text_startup_instruction_screen_2[];

extern const char *ui_text_startup_instruction_screen_3[];

extern const char *ui_text_startup_instruction_screen_4[];

extern const char *ui_text_view_data;

// errors
extern const char *ui_text_something_went_wrong;
extern const char *ui_text_something_went_wrong_contact_support_send_logs;
extern const char *ui_text_already_have_maxi_wallets;
extern const char *ui_text_aborted;
extern const char *ui_text_operation_has_been_cancelled;
extern const char *ui_text_wallet_name_exists;
extern const char *ui_text_pin_incorrect_re_enter;
extern const char *ui_text_wallet_with_same_mnemo_exists;
extern const char *ui_text_incorrect_mnemonics;
extern const char *ui_text_incorrect_choice_view_seed_again;
extern const char *ui_text_xpub_not_found_add_coin_first;
extern const char *ui_wallet_pin_instruction_2;
extern const char *ui_wallet_passphrase_instruction_2;
extern const char *ui_wallet_passphrase_instruction_3;
extern const char *ui_wallet_passphrase_instruction_4;
extern const char *ui_text_no_response_from_desktop;
extern const char *ui_text_invalid_transaction;

// errors card flows
extern const char *ui_text_invalid_card_contact_cypherock;
extern const char *ui_text_wrong_card_sequence;
extern const char *ui_text_tap_another_card;
extern const char *ui_text_wallet_doesnt_exists_on_this_card;
extern const char *ui_text_wrong_wallet_is_now_locked;
extern const char *ui_text_wallet_already_unlocked;
extern const char *ui_text_wallet_verification_failed_in_creation;
extern const char *ui_text_wallet_verification_failed_in_reconstruction;

extern const char *ui_text_invalid_card_tap_card[];
extern const char *ui_text_device_authenticating[];
extern const char *ui_text_device_provisioning[];

extern const char *ui_text_tap_a_card_instruction1;
extern const char *ui_text_tap_a_card_instruction2;
extern const char *ui_text_tap_a_card;

extern const char *ui_text_tap_1_2_cards;
extern const char *ui_text_tap_2_2_cards;
extern const char *ui_text_place_card_below;

extern const char *ui_text_card_is_full;

extern const char *ui_text_device_synced_with_card;
extern const char *ui_text_family_id_retrived;
extern const char *ui_text_family_id_mismatch;

extern const char *ui_text_hash_not_accepted_by_card;

extern const char *ui_text_do_not_detach_device;

extern const char *ui_text_card_command_send_error;
extern const char *ui_text_card_update_done;
extern const char *ui_text_card_transaction_exception;
extern const char *ui_text_card_out_of_boundary_exception;
extern const char *ui_text_card_null_pointer_exception;
extern const char *ui_text_card_crypto_exception;
extern const char *ui_text_card_invalid_apdu_length;
extern const char *ui_text_card_invalid_tag_in_apdu;
extern const char *ui_text_unknown_error_contact_support;
extern const char *ui_text_unauthenticated_device;

// headings
extern const char *ui_text_enter_pin;
extern const char *ui_text_confirm_pin;
extern const char *ui_text_word_hash;
extern const char *ui_text_verify_word_hash;
extern const char *ui_text_enter_wallet_name;
extern const char *ui_text_number_of_words;
extern const char *ui_text_add_coins;
extern const char *ui_text_resync_coins;
extern const char *ui_text_choose_wallet;
extern const char *ui_text_enter_passphrase;
extern const char *ui_text_confirm_passphrase;
extern const char *ui_text_receive_on;
extern const char *ui_text_verify_address;
extern const char *ui_text_verify_amount;
extern const char *ui_text_verify_contract;
extern const char *ui_text_unverified_contract;
extern const char *ui_text_confirm_wallet_name;
extern const char *ui_text_enter_data;
extern const char *ui_text_confirm_data;
extern const char *ui_text_verification_cancelled;
extern const char *ui_text_verify_nonce;
extern const char *ui_text_waiting_for_desktop;

// headings near specific
extern const char *ui_text_verify_create_from;
extern const char *ui_text_verify_new_account_id;
extern const char *ui_text_new_account_id;
extern const char *ui_text_new_public_key;
extern const char *ui_text_replace_account;
extern const char *ui_text_confirm_account;
extern const char *ui_text_near_transfer_action_type;
extern const char *ui_text_near_create_account_method;

// headings card flow
extern const char *ui_text_family_id_hex;

// messages
extern const char *ui_text_use_passphrase_question;
extern const char *ui_text_wallet_name_size_limit;
extern const char *ui_text_check_cysync;
extern const char *ui_text_tap_x1card_auth_instruction_1;
extern const char *ui_text_tap_x1card_auth_instruction_2;
extern const char *ui_text_tap_card_to_start;
extern const char *ui_text_wallet_deleted_successfully;
extern const char *ui_text_recovery_successfull;
extern const char *ui_text_processing;
extern const char *ui_text_press_enter_to_start_verification;
extern const char *ui_text_now_enter_your_seed_phrase;
extern const char *ui_text_now_enter_your_data;
extern const char *ui_text_verify_entered_words;
extern const char *ui_text_exported_signed_transaction_to_desktop;
extern const char *ui_text_start_card_update;
extern const char *ui_text_start_verification_of_card;
extern const char *ui_text_wallet_lock_continue_to_unlock;
extern const char *ui_text_wallet_partial_continue_to_delete;
extern const char *ui_text_wallet_not_verified_continue_to_verify;
extern const char *ui_text_wallet_out_of_sync_continue_to_sync_with_x1cards;
extern const char *ui_text_wallet_visit_to_verify;
extern const char *ui_text_wallet_partial_fix;
#ifdef ALLOW_LOG_EXPORT
extern const char *ui_text_sending_logs;
extern const char *ui_text_logs_sent;
#endif
extern const char *ui_text_creation_failed_delete_wallet;

extern const char *ui_text_successfull;
extern const char *ui_text_view_seed_messages;
extern const char *ui_text_card_authentication_success;
extern const char *ui_text_card_authentication_failed;
extern const char *ui_text_authentication_required;
extern const char *ui_text_start_auth_from_CySync;
extern const char *ui_text_enable_passphrase_step;
extern const char *ui_text_disable_passphrase_step;
extern const char *ui_text_warning_txn_fee_too_high;
extern const char *ui_text_enable_log_export;
extern const char *ui_text_disable_log_export;

//?
extern const char *ui_text_do_you_want_import_wallet_to_desktop;
extern const char *ui_text_do_you_want_to_set_pin;

extern const char *ui_text_auth_process;

extern const char *ui_text_sync_x1card_confirm;

extern const char *ui_text_no_wallets_present;
extern const char *ui_text_device_verification_success;
extern const char *ui_text_device_verification_failure;
extern const char *ui_text_unauthenticate_via_cysync;
extern const char *ui_text_start_device_verification;
extern const char *ui_text_its_a_while_check_your_cards;
extern const char *ui_text_delete_this_wallet;
extern const char *ui_text_need_all_x1cards_to_create_wallet;
extern const char *ui_text_need_all_x1cards_to_delete_wallet_entirely;
extern const char *ui_text_20_spaces;
extern const char *ui_text_wallet_not_found_on_x1card;
extern const char *ui_text_sync_wallets_next_steps;
extern const char *ui_text_corrupted_card_contact_support;
extern const char *ui_text_incompatible_card_version;
extern const char *ui_text_card_error_contact_support;
extern const char *ui_text_device_and_card_not_paired;
extern const char *ui_text_security_conditions_not_met;
extern const char *ui_text_retry_or_repair;
extern const char *ui_text_process_reset_due_to_inactivity;
extern const char *ui_text_device_already_provisioned;
extern const char *ui_text_check_cysync_app;
extern const char *ui_text_provision_success;
extern const char *ui_text_provision_fail;
extern const char *ui_text_cannot_verify_card_contact_support;
extern const char *ui_text_wrong_btc_transaction;
extern const char *ui_text_btc_change_address_mismatch;
extern const char *ui_text_worng_eth_transaction;
extern const char *ui_text_error_no_card_paired;
extern const char *ui_text_error_pair_all_cards;
extern const char *ui_text_error_pair_atleast_2_cards;
extern const char *ui_text_continue_with_pairing;
extern const char *ui_text_device_compromised;
extern const char *ui_text_device_compromised_not_provisioned;
extern const char *ui_text_device_compromised_partially_provisioned;
extern const char *ui_text_device_compromised_v1_config;
extern const char *ui_text_pow_challenge_failed;
extern const char *ui_text_card_removed_fast;
extern const char *ui_text_card_freq_discon_fault;
extern const char *ui_text_card_align_with_device_screen;
extern const char *ui_text_remove_card_prompt;
extern const char *ui_text_card_detected;
extern const char *ui_text_nfc_hardware_fault_detected;
extern const char *ui_text_unreliable_cards;
extern const char *ui_critical_card_health_migrate_data;

// App specific

// Inheritance
extern const char *ui_text_inheritance_wallet_auth_flow_confirmation;
extern const char *ui_text_inheritance_wallet_auth_flow_confirmation_generic;
extern const char *ui_text_inheritance_wallet_authenticating;
extern const char *ui_text_inheritance_wallet_auth_success;
extern const char *ui_text_inheritance_wallet_auth_fail;

extern const char *ui_text_inheritance_encryption_flow_confirmation;
extern const char *ui_text_inheritance_encryption_flow_success;
extern const char *ui_text_inheritance_encryption_flow_failure;

extern const char *ui_text_inheritance_decryption_flow_confirmation;
extern const char *ui_text_inheritance_decryption_flow_confirmation_generic;
extern const char *ui_text_inheritance_decryption_flow_success;
extern const char *ui_text_inheritance_decryption_flow_failure;

// ICP

extern const char *ui_text_principal_id;
extern const char *ui_text_account_id;

#ifdef ALLOW_LOG_EXPORT
extern const char *ui_text_send_logs_prompt;
#endif
#endif    // CONSTANT_TEXTS_H
