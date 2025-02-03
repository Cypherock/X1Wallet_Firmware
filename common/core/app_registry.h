/**
 * @file    app_registry.h
 * @author  Cypherock X1 Team
 * @brief   App descriptor registry APIs for application to work with.
 * @copyright Copyright (c) 2023 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 * target=_blank>https://mitcc.org/</a>
 */
#ifndef APP_REGISTRY_H
#define APP_REGISTRY_H

/*****************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "core.pb.h"
#include "events.h"

/*****************************************************************************
 * MACROS AND DEFINES
 *****************************************************************************/

#define REGISTRY_MAX_APPS 23

/*****************************************************************************
 * TYPEDEFS
 *****************************************************************************/
typedef void (*app_entry)(usb_event_t, const void *);

typedef struct cy_app_desc {
  const uint32_t id;
  const common_version_t version;

  const app_entry app;
  const void *app_config;
} cy_app_desc_t;

/*****************************************************************************
 * EXPORTED VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *****************************************************************************/
/**
 * The function `registry_add_app` adds an application descriptor to a registry,
 * ensuring that there are no collisions with existing descriptors and that the
 * registry does not overflow.
 *
 * @param app_desc The parameter `app_desc` is a pointer to a structure of type
 * `cy_app_desc_t`.
 *
 * @return a boolean value.
 */
bool registry_add_app(const cy_app_desc_t *app_desc);

/**
 * The function "registry_get_app_desc" returns a pointer to the app descriptor
 * for a given app ID.
 *
 * @param app_id The app_id parameter is an unsigned 32-bit integer that
 * represents the ID of the application.
 *
 * @return a pointer to a constant structure of type `cy_app_desc_t`.
 */
const cy_app_desc_t *registry_get_app_desc(uint32_t app_id);

/**
 * The function "registry_get_app_list" returns a pointer to a list of app
 * descriptors.
 *
 * @return The function `registry_get_app_list` is returning a pointer to a
 * pointer to a `cy_app_desc_t` structure.
 */
const cy_app_desc_t **registry_get_app_list();
#endif
