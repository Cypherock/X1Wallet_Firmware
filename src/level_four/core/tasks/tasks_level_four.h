/**
 * @file    tasks_level_four.h
 * @author  Cypherock X1 Team
 * @brief   Header for level four tasks.
 *          This file contains all the functions prototypes for level four tasks.
 * @copyright Copyright (c) 2022 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/" target=_blank>https://mitcc.org/</a>
 * 
 */
#ifndef TASKS_LEVEL_FOUR_H
#define TASKS_LEVEL_FOUR_H

#pragma once

#include "controller_main.h"
#include "tasks.h"
#include "eth.h"

extern eth_unsigned_txn eth_unsigned_txn_ptr;
extern uint8_t challenge_no[32];

/**
 * @brief This task is executed for exporting wallet on device
 * @details
 *
 * @see EXPORT_WALLET_TASKS,
 * @since v1.0.0
 */
void export_wallet_task();

/**
 * @brief This task is executed for adding xpub to the device.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void add_coin_tasks();

/**
 * @brief This task is executed for processing and signing unsigned transaction.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void send_transaction_tasks();

/**
 * @brief This task is executed for processing and signing unsigned transaction.
 */
void send_transaction_tasks_near();

/**
 * @brief This task is executed for processing and signing unsigned transaction 
 *  of Ethereum.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void send_transaction_tasks_eth();

/**
 * @brief This task is executed for processing and signing unsigned transaction.
 */
void send_transaction_tasks_solana();

/**
 * @brief This task is executed for generating address using xpub.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void receive_transaction_tasks();

/**
 * @brief This task is executed for generating address using xpub for Ethereum.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void receive_transaction_tasks_eth();

/**
 * @brief This task is executed for handling near registered accounts and generating near implicit account.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void receive_transaction_tasks_near();

/**
 * @brief This task is executed for generating solana address using the public key
 * @details
 *
 * @see
 * @since v1.0.0
 */
void receive_transaction_tasks_solana();

/**
 * @brief This task is executed for verifying wallet added or restored
 * on the device and cards.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void verify_wallet_tasks();

/**
 * @brief Device authentication task handler
 * @details
 *
 * @see
 * @since v1.0.0
 */
void task_device_authentication();

/**
 * @brief Device provision task handler
 * @details
 *
 * @see
 * @since v1.0.0
 */
void task_device_provision();

/**
 * @brief Card verification task for initial flow (includes pairing step)
 * @details
 *
 * @see
 * @since v1.0.0
 */
void initial_verify_card_task();

/**
 * @brief This task is executed when the name of a wallet is clicked
 * and it is locked.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void wallet_locked_task();

/**
 * @brief This task is executed for verifying card.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void verify_card_task();

/**
 * @brief This task is executed for upgrading cards.
 * @details
 *
 * @see
 * @since v1.0.0
 */
void card_upgrade_task();

/**
 * @brief
 * @details
 *
 * @see
 * @since v1.0.0
 */
void sync_cards_task();

#endif