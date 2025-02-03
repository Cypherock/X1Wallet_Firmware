/**
 * @file    core_flow_init.c
 * @author  Cypherock X1 Team
 * @brief
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
#include "core_flow_init.h"

#include "app_registry.h"
#include "application_startup.h"
#include "arbitrum_app.h"
#include "avalanche_app.h"
#include "bsc_app.h"
#include "btc_app.h"
#include "btc_main.h"
#include "dash_app.h"
#include "doge_app.h"
#include "eth_app.h"
#include "evm_main.h"
#include "fantom_app.h"
#include "inheritance_main.h"
#include "ltc_app.h"
#include "main_menu.h"
#include "manager_app.h"
#include "near_main.h"
#include "onboarding.h"
#include "optimism_app.h"
#include "polygon_app.h"
#include "restricted_app.h"
#include "solana_main.h"
#include "tron_main.h"
#include "xrp_main.h"
#include "icp_main.h"

/*****************************************************************************
 * EXTERN VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * PRIVATE MACROS AND DEFINES
 *****************************************************************************/
#define CORE_ENGINE_BUFFER_SIZE 10

/*****************************************************************************
 * PRIVATE TYPEDEFS
 *****************************************************************************/

/*****************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/
flow_step_t *core_step_buffer[CORE_ENGINE_BUFFER_SIZE] = {0};
engine_ctx_t core_step_engine_ctx = {
    .array = &core_step_buffer[0],
    .current_index = 0,
    .max_capacity = sizeof(core_step_buffer) / sizeof(core_step_buffer[0]),
    .num_of_elements = 0,
    .size_of_element = sizeof(core_step_buffer[0])};

/*****************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTIONS
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTIONS
 *****************************************************************************/
engine_ctx_t *get_core_flow_ctx(void) {
  engine_reset_flow(&core_step_engine_ctx);

  const manager_onboarding_step_t step = onboarding_get_last_step();
  /// Check if onboarding is complete or not
  if (MANAGER_ONBOARDING_STEP_COMPLETE != step) {
    // reset partial-onboarding if auth flag is reset (which can happen via
    // secure-bootloader). Refer PRF-7078
    if (MANAGER_ONBOARDING_STEP_VIRGIN_DEVICE < step &&
        DEVICE_NOT_AUTHENTICATED == get_auth_state()) {
      // bypass onboarding_set_step_done as we want to force reset
      save_onboarding_step(MANAGER_ONBOARDING_STEP_VIRGIN_DEVICE);
    }

    // Skip onbaording for infield devices with pairing and/or wallets count is
    // greater than zero
    if ((get_wallet_count() > 0) || (get_keystore_used_count() > 0)) {
      onboarding_set_step_done(MANAGER_ONBOARDING_STEP_COMPLETE);
    } else {
      engine_add_next_flow_step(&core_step_engine_ctx, onboarding_get_step());
      return &core_step_engine_ctx;
    }
  }

  // Check if device needs to go to restricted state or not
  if (DEVICE_AUTHENTICATED != get_auth_state()) {
    engine_add_next_flow_step(&core_step_engine_ctx, restricted_app_get_step());
    return &core_step_engine_ctx;
  }

  if (MANAGER_ONBOARDING_STEP_COMPLETE == get_onboarding_step() &&
      DEVICE_AUTHENTICATED == get_auth_state()) {
    check_invalid_wallets();
  }

  // Finally enable all flows from the user
  engine_add_next_flow_step(&core_step_engine_ctx, main_menu_get_step());
  return &core_step_engine_ctx;
}

void core_init_app_registry() {
  registry_add_app(get_manager_app_desc());
  registry_add_app(get_btc_app_desc());
  registry_add_app(get_ltc_app_desc());
  registry_add_app(get_doge_app_desc());
  registry_add_app(get_dash_app_desc());
  registry_add_app(get_eth_app_desc());
  registry_add_app(get_near_app_desc());
  registry_add_app(get_polygon_app_desc());
  registry_add_app(get_solana_app_desc());
  registry_add_app(get_bsc_app_desc());
  registry_add_app(get_fantom_app_desc());
  registry_add_app(get_avalanche_app_desc());
  registry_add_app(get_optimism_app_desc());
  registry_add_app(get_arbitrum_app_desc());
  registry_add_app(get_tron_app_desc());
  registry_add_app(get_inheritance_app_desc());
  registry_add_app(get_xrp_app_desc());
  registry_add_app(get_icp_app_desc());
}
