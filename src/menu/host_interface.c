/**
 * @file    host_interface.c
 * @author  Cypherock X1 Team
 * @brief   Source file for the main-menu host interface
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

#include "host_interface.h"

#include "btc_app.h"
#include "btc_main.h"
#include "main_menu.h"
#include "manager_app.h"
#include "status_api.h"

/*****************************************************************************
 * EXTERN VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * PRIVATE MACROS AND DEFINES
 *****************************************************************************/

/*****************************************************************************
 * PRIVATE TYPEDEFS
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *****************************************************************************/

/*****************************************************************************
 * STATIC VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * STATIC FUNCTIONS
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTIONS
 *****************************************************************************/

void main_menu_host_interface(engine_ctx_t *ctx,
                              usb_event_t usb_evt,
                              const void *data) {
  /* TODO: A USB request was detected by the core, but it was the first time
   * this request came in, therefore, we will pass control to the required
   * application here */

  uint32_t applet_id = get_applet_id();
  switch (applet_id) {
    case 1: {
      manager_app_main(usb_evt);
      break;
    }
    case 2: {
      // TODO: instantiate & configure Bitcoin chain
      btc_main(usb_evt, get_btc_app());
      break;
    }
    case 3: {
      // TODO: We might conditionally allow support Bitcoin testnet
      // TODO: instantiate & configure Bitcoin testnet chain
      btc_main(usb_evt, get_btc_app());
      break;
    }
    case 4: {
      // TODO: instantiate & configure Litecoin chain
      btc_main(usb_evt, get_btc_app());
      break;
    }
    case 5: {
      // TODO: instantiate & configure Dogecoin chain
      btc_main(usb_evt, get_btc_app());
      break;
    }
    case 6: {
      // TODO: instantiate & configure Dash chain
      btc_main(usb_evt, get_btc_app());
      break;
    }
    default: {
      // TODO: send core error about invalid applet id
      break;
    }
  }

  main_menu_set_update_req(true);
  return;
}