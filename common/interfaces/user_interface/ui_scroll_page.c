/**
 * @file    ui_scroll_page.c
 * @author  Cypherock X1 Team
 * @brief   Title of the file.
 *          Short description of the file
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
#include "ui_scroll_page.h"

#include <stdint.h>

#include "stdlib.h"
#include "ui_events_priv.h"
#ifdef DEV_BUILD
#include "dev_utils.h"
#endif

scrolling_page_data_t *gp_scrollabe_page_data = NULL;
scrolling_page_lvgl_t *gp_scrollabe_page_lvgl = NULL;

/**
 * @brief This function increments the current page
 * gp_scrollabe_page_data->curr_page_num
 *
 * @return true: In case the page was incremented
 * @return false: In case the page was not incremented as there are no pages
 * remaining
 */
static bool page_increment(void);

/**
 * @brief This function decrements the current page
 * gp_scrollabe_page_data->curr_page_num
 *
 * @return true: In case the page was incremented
 * @return false: In case the page was not incremented as there are no pages
 * remaining
 */
static bool page_decrement(void);

/**
 * @brief This function updates the UI elements like left/right arrows,
 * cancel/accepts buttons and footnote on the screen
 *
 */
static void page_update_icons(void);

/**
 * @brief This function hides or shows the left and right arrows based on
 * current page number
 *
 */
static void page_update_arrows(void);

/**
 * @brief This function hides or shows the cancel or accept buttons on the last
 * page If the buttons are visible, it focuses on the accept button by default
 */
static void page_update_buttons(void);

/**
 * @brief This function updates the text in the footnote of the screen
 *
 */
static void page_update_footnote(void);

/**
 * @brief This function cleans the global memory used by the
 * ui_scrollable_page() and clears the screen
 *
 */
static void ui_scrollable_destructor(void);

/**
 * @brief This function handles an event emitted on the cancel button
 *
 * @param pCancelLvglObj: The pointer to lvgl cancel button object
 * @param lvglEvent: The event emitted
 */
static void page_cancel_handler(lv_obj_t *pCancelLvglObj,
                                const lv_event_t lvglEvent);

/**
 * @brief This function handles an event emitted on the accept button
 *
 * @param pCancelLvglObj: The pointer to lvgl accept button object
 * @param lvglEvent: The event emitted
 */
static void page_accept_handler(lv_obj_t *pAcceptLvglObj,
                                const lv_event_t lvglEvent);

/**
 * @brief This function handles an event emitted on the body
 *
 * @param pCancelLvglObj: The pointer to lvgl body object
 * @param lvglEvent: The event emitted
 */
static void page_arrow_handler(lv_obj_t *pLvglArrowObject,
                               const lv_event_t lvglEvent);

/**
 * @brief This function populates LVGL objects in gp_scrollabe_page_lvgl
 * variable for a UI screen which is scrollabe, contains left/right arrow
 * buttons and cancel/accept buttons Note: Before calling this function:
 * gp_scrollabe_page_data variable should be malloced and populated with
 * appropriate values
 */
static void ui_scrollable_page_create(void);

static bool page_increment(void) {
  ASSERT(NULL != gp_scrollabe_page_data);

  if (gp_scrollabe_page_data->curr_page_num <
      gp_scrollabe_page_data->total_page_num) {
    gp_scrollabe_page_data->curr_page_num += 1;
    return true;
  }

  return false;
}

static bool page_decrement(void) {
  ASSERT(NULL != gp_scrollabe_page_data);

  if (gp_scrollabe_page_data->curr_page_num > 1) {
    gp_scrollabe_page_data->curr_page_num -= 1;
    return true;
  }

  return false;
}

static void page_update_arrows(void) {
  ASSERT((NULL != gp_scrollabe_page_data) && (NULL != gp_scrollabe_page_lvgl));

  gp_scrollabe_page_data->bool_left_arrow_hidden = true;
  gp_scrollabe_page_data->bool_right_arrow_hidden = true;

  /* If we have remaining pages on the left, bool_left_arrow_hidden = false */
  if (gp_scrollabe_page_data->curr_page_num > 1) {
    gp_scrollabe_page_data->bool_left_arrow_hidden = false;
  }

  /* If we have remaining pages on the right, bool_right_arrow_hidden = false */
  if (gp_scrollabe_page_data->curr_page_num <
      gp_scrollabe_page_data->total_page_num) {
    gp_scrollabe_page_data->bool_right_arrow_hidden = false;
  }

  lv_obj_set_hidden(gp_scrollabe_page_lvgl->p_ui_left_arrow_lvgl,
                    gp_scrollabe_page_data->bool_left_arrow_hidden);
  lv_obj_set_hidden(gp_scrollabe_page_lvgl->p_ui_right_arrow_lvgl,
                    gp_scrollabe_page_data->bool_right_arrow_hidden);

  return;
}

static void page_update_buttons(void) {
  ASSERT((NULL != gp_scrollabe_page_data) && (NULL != gp_scrollabe_page_lvgl));

  gp_scrollabe_page_data->bool_accept_cancel_hidden = true;

  /* Cancel/Accept buttons are only visible if we are on the last page */
  if (gp_scrollabe_page_data->curr_page_num ==
      gp_scrollabe_page_data->total_page_num) {
    gp_scrollabe_page_data->bool_accept_cancel_hidden = false;
  }

  /**
   * Override last page check if
   * gp_scrollabe_page_data->bool_accept_cancel_visible == true as caller has
   * requested accept/cancel button on all pages
   */
  if (true == gp_scrollabe_page_data->bool_accept_cancel_visible) {
    gp_scrollabe_page_data->bool_accept_cancel_hidden = false;
  }

  lv_obj_set_hidden(gp_scrollabe_page_lvgl->p_ui_cancel_btn_lvgl,
                    gp_scrollabe_page_data->bool_accept_cancel_hidden);
  lv_obj_set_hidden(gp_scrollabe_page_lvgl->p_ui_accept_btn_lvgl,
                    gp_scrollabe_page_data->bool_accept_cancel_hidden);

  /* If we are on the last page, then highlight the accept button by default */
  if (gp_scrollabe_page_data->curr_page_num ==
      gp_scrollabe_page_data->total_page_num) {
    lv_group_focus_obj(gp_scrollabe_page_lvgl->p_ui_accept_btn_lvgl);
  }

  return;
}

static void page_update_header(void) {
  if (!gp_scrollabe_page_data->bool_only_first_page_header_visible) {
    return;
  }

  ASSERT((NULL != gp_scrollabe_page_data) && (NULL != gp_scrollabe_page_lvgl));

  bool is_heading_hidden = gp_scrollabe_page_data->curr_page_num != 1;

  lv_coord_t scroll_page_height = 48;
  lv_align_t scroll_page_aligment = LV_ALIGN_IN_TOP_MID;

  if (!is_heading_hidden) {
    scroll_page_height = 32;
    scroll_page_aligment = LV_ALIGN_CENTER;
  }

  lv_obj_set_size(
      gp_scrollabe_page_lvgl->p_ui_page_lvgl, 128, scroll_page_height);
  lv_obj_align(
      gp_scrollabe_page_lvgl->p_ui_page_lvgl, NULL, scroll_page_aligment, 0, 0);
  lv_obj_set_hidden(gp_scrollabe_page_lvgl->p_ui_header_lvgl,
                    is_heading_hidden);
}

static void page_update_footnote(void) {
  ASSERT((NULL != gp_scrollabe_page_data) && (NULL != gp_scrollabe_page_lvgl));

  if (NULL != gp_scrollabe_page_lvgl->p_ui_footnote_lvgl) {
    snprintf(gp_scrollabe_page_data->p_ui_footnote,
             MAXIMUM_CHARACTERS_IN_FOOTNOTE,
             "%d/%d",
             gp_scrollabe_page_data->curr_page_num,
             gp_scrollabe_page_data->total_page_num);
    ui_paragraph(gp_scrollabe_page_lvgl->p_ui_footnote_lvgl,
                 gp_scrollabe_page_data->p_ui_footnote,
                 LV_LABEL_ALIGN_CENTER);
    lv_obj_align(gp_scrollabe_page_lvgl->p_ui_footnote_lvgl,
                 NULL,
                 LV_ALIGN_IN_BOTTOM_MID,
                 0,
                 0);
  }

  return;
}

static void page_update_icons(void) {
  page_update_arrows();
  page_update_buttons();
  page_update_footnote();
  return;
}

static void ui_scrollable_destructor(void) {
  if (NULL != gp_scrollabe_page_data) {
    memzero(gp_scrollabe_page_data, sizeof(scrolling_page_data_t));
    free(gp_scrollabe_page_data);
    gp_scrollabe_page_data = NULL;
  }

  if (NULL != gp_scrollabe_page_lvgl) {
    memzero(gp_scrollabe_page_lvgl, sizeof(scrolling_page_lvgl_t));
    free(gp_scrollabe_page_lvgl);
    gp_scrollabe_page_lvgl = NULL;
  }

  return;
}

static void page_cancel_handler(lv_obj_t *pCancelLvglObj,
                                const lv_event_t lvglEvent) {
  if ((LV_EVENT_DELETE != lvglEvent) && (lv_obj_get_hidden(pCancelLvglObj))) {
    return;
  }

  switch (lvglEvent) {
    case LV_EVENT_KEY: {
      lv_key_t keyPressed = lv_indev_get_key(ui_get_indev());
      if (LV_KEY_RIGHT == keyPressed) {
        lv_group_focus_obj(gp_scrollabe_page_lvgl->p_ui_accept_btn_lvgl);
      } else if (LV_KEY_LEFT == keyPressed) {
        /**
         * If the cancel icon is pressed and the user moves joystick to left, we
         * should scroll to the previous page (if any)
         * So manually call page_arrow_handler(); for this special case.
         */
        lv_group_focus_obj(gp_scrollabe_page_lvgl->p_ui_body_lvgl);
        page_arrow_handler(gp_scrollabe_page_lvgl->p_ui_body_lvgl,
                           LV_EVENT_KEY);
      }
      break;
    }
    case LV_EVENT_CLICKED: {
      ui_set_cancel_event();
      break;
    }
    case LV_EVENT_DEFOCUSED: {
      lv_btn_set_state(pCancelLvglObj, LV_BTN_STATE_REL);
      break;
    }
    case LV_EVENT_DELETE: {
      /* Destruct object and data variables in case the object is being deleted
       * directly using lv_obj_clean() */
      ui_scrollable_destructor();
      break;
    }
    default: {
      break;
    }
  }

  return;
}

static void page_accept_handler(lv_obj_t *pAcceptLvglObj,
                                const lv_event_t lvglEvent) {
  if ((LV_EVENT_DELETE != lvglEvent) && (lv_obj_get_hidden(pAcceptLvglObj))) {
    return;
  }

  switch (lvglEvent) {
    case LV_EVENT_KEY: {
      if (LV_KEY_LEFT == lv_indev_get_key(ui_get_indev())) {
        lv_group_focus_obj(gp_scrollabe_page_lvgl->p_ui_cancel_btn_lvgl);
      }
      break;
    }
    case LV_EVENT_CLICKED: {
      ui_set_confirm_event();
      break;
    }
    case LV_EVENT_DEFOCUSED: {
      lv_btn_set_state(pAcceptLvglObj, LV_BTN_STATE_REL);
      break;
    }
    case LV_EVENT_DELETE: {
      /* Destruct object and data variables in case the object is being deleted
       * directly using lv_obj_clean() */
      ui_scrollable_destructor();
      break;
    }
    default: {
      break;
    }
  }

  return;
}

static void page_arrow_handler(lv_obj_t *pLvglArrowObject,
                               const lv_event_t lvglEvent) {
  switch (lvglEvent) {
    case LV_EVENT_KEY: {
      if (LV_BTN_STATE_PR == lv_btn_get_state(pLvglArrowObject)) {
        lv_label_set_style(gp_scrollabe_page_lvgl->p_ui_right_arrow_lvgl,
                           LV_LABEL_STYLE_MAIN,
                           &(gp_scrollabe_page_lvgl->ui_arrow_released_style));
        lv_label_set_style(gp_scrollabe_page_lvgl->p_ui_left_arrow_lvgl,
                           LV_LABEL_STYLE_MAIN,
                           &(gp_scrollabe_page_lvgl->ui_arrow_released_style));
      }
      lv_key_t keyPressed = lv_indev_get_key(ui_get_indev());
      if (LV_KEY_RIGHT == keyPressed) {
        if (true == page_increment()) {
          lv_label_set_style(gp_scrollabe_page_lvgl->p_ui_right_arrow_lvgl,
                             LV_LABEL_STYLE_MAIN,
                             &(gp_scrollabe_page_lvgl->ui_arrow_pressed_style));

          lv_obj_t *p_scrollable =
              lv_page_get_scrl(gp_scrollabe_page_lvgl->p_ui_page_lvgl);
          lv_coord_t page_y = lv_obj_get_y(p_scrollable);
          lv_obj_set_y(p_scrollable,
                       page_y - lv_obj_get_height(
                                    gp_scrollabe_page_lvgl->p_ui_page_lvgl));

          lv_obj_align(gp_scrollabe_page_lvgl->p_ui_body_lvgl,
                       NULL,
                       LV_ALIGN_IN_TOP_MID,
                       0,
                       0);
          page_update_header();
          page_update_icons();
        }
      } else if (LV_KEY_LEFT == keyPressed) {
        if (true == page_decrement()) {
          /***
           * We have to update header before calculating scroll distance
           * other wise the current page height is decrease instead of previous
           * page height
           */
          page_update_header();
          lv_label_set_style(gp_scrollabe_page_lvgl->p_ui_left_arrow_lvgl,
                             LV_LABEL_STYLE_MAIN,
                             &(gp_scrollabe_page_lvgl->ui_arrow_pressed_style));
          lv_obj_t *p_scrollable =
              lv_page_get_scrl(gp_scrollabe_page_lvgl->p_ui_page_lvgl);
          lv_coord_t page_y = lv_obj_get_y(p_scrollable);
          lv_obj_set_y(p_scrollable,
                       page_y + lv_obj_get_height(
                                    gp_scrollabe_page_lvgl->p_ui_page_lvgl));

          lv_obj_align(gp_scrollabe_page_lvgl->p_ui_body_lvgl,
                       NULL,
                       LV_ALIGN_IN_TOP_MID,
                       0,
                       0);
          page_update_icons();
        }
      }

      break;
    }

    case LV_EVENT_RELEASED: {
      page_update_icons();
      lv_label_set_style(gp_scrollabe_page_lvgl->p_ui_right_arrow_lvgl,
                         LV_LABEL_STYLE_MAIN,
                         &(gp_scrollabe_page_lvgl->ui_arrow_released_style));
      lv_label_set_style(gp_scrollabe_page_lvgl->p_ui_left_arrow_lvgl,
                         LV_LABEL_STYLE_MAIN,
                         &(gp_scrollabe_page_lvgl->ui_arrow_released_style));
      break;
    }

    case LV_EVENT_DELETE: {
      /* Destruct object and data variables in case the object is being
       * deleted directly using lv_obj_clean() */
      ui_scrollable_destructor();
      break;
    }

    default: {
      break;
    }
  }

  return;
}

static void ui_scrollable_page_create(void) {
  ASSERT(NULL != gp_scrollabe_page_data);

  gp_scrollabe_page_lvgl =
      (scrolling_page_lvgl_t *)malloc(sizeof(scrolling_page_lvgl_t));
  ASSERT(NULL != gp_scrollabe_page_lvgl);

  lv_coord_t scroll_page_height = 48;
  lv_align_t scroll_page_aligment = LV_ALIGN_IN_TOP_MID;

  /* Accomodate cases where heading is not NULL */
  if (NULL != gp_scrollabe_page_data->p_ui_heading) {
    // 16 pixels will be consumed by the heading, so height of scrollable text
    // would be less
    scroll_page_height = 32;
    scroll_page_aligment = LV_ALIGN_CENTER;

    /* Create label gp_scrollabe_page_lvgl->p_ui_header_lvgl which stores the
     * heading at the top of the screen */
    gp_scrollabe_page_lvgl->p_ui_header_lvgl =
        lv_label_create(lv_scr_act(), NULL);

    ui_heading(gp_scrollabe_page_lvgl->p_ui_header_lvgl,
               gp_scrollabe_page_data->p_ui_heading,
               LV_HOR_RES - 20,
               LV_LABEL_ALIGN_CENTER);
    lv_style_copy(&(gp_scrollabe_page_lvgl->ui_header_style), &lv_style_plain);
    (gp_scrollabe_page_lvgl->ui_header_style).body.padding.bottom = 1;
    (gp_scrollabe_page_lvgl->ui_header_style).body.border.width = 1;
    (gp_scrollabe_page_lvgl->ui_header_style).body.border.part =
        LV_BORDER_BOTTOM;
    lv_label_set_style(gp_scrollabe_page_lvgl->p_ui_header_lvgl,
                       LV_LABEL_STYLE_MAIN,
                       &(gp_scrollabe_page_lvgl->ui_header_style));
    lv_label_set_body_draw(gp_scrollabe_page_lvgl->p_ui_header_lvgl, true);
  }

  /* Create a page gp_scrollabe_page_lvgl->p_ui_page_lvgl of size
   * 128xpage_height pixels in the middle of the screen */
  gp_scrollabe_page_lvgl->p_ui_page_lvgl = lv_page_create(lv_scr_act(), NULL);
  lv_obj_set_size(
      gp_scrollabe_page_lvgl->p_ui_page_lvgl, 128, scroll_page_height);
  /* Style the page to have no border and disable scrollbar visibility */
  lv_page_set_style(gp_scrollabe_page_lvgl->p_ui_page_lvgl,
                    LV_PAGE_STYLE_BG,
                    &lv_style_transp);
  lv_page_set_style(gp_scrollabe_page_lvgl->p_ui_page_lvgl,
                    LV_PAGE_STYLE_SCRL,
                    &lv_style_transp_fit);
  lv_page_set_sb_mode(gp_scrollabe_page_lvgl->p_ui_page_lvgl, LV_SB_MODE_OFF);
  lv_obj_align(
      gp_scrollabe_page_lvgl->p_ui_page_lvgl, NULL, scroll_page_aligment, 0, 0);

  /**
   * Create a label on page gp_scrollabe_page_lvgl->p_ui_page_lvgl which
   * contains the body holding the actual text Size of the label is
   * lv_page_get_fit_width(gp_scrollabe_page_lvgl->p_ui_page_lvgl) - 16,
   * lv_page_get_fit_height(gp_scrollabe_page_lvgl->p_ui_page_lvgl) Text will
   * be broken into multiple lines, but only 2 or 3 lines (32 pixels/ 48
   * pixels) are available on the page. So this creates a scrollable label on
   * the page.
   */
  gp_scrollabe_page_lvgl->p_ui_body_lvgl =
      lv_label_create(gp_scrollabe_page_lvgl->p_ui_page_lvgl, NULL);
  lv_label_set_long_mode(gp_scrollabe_page_lvgl->p_ui_body_lvgl,
                         LV_LABEL_LONG_BREAK);
  lv_obj_set_size(
      gp_scrollabe_page_lvgl->p_ui_body_lvgl,
      lv_page_get_fit_width(gp_scrollabe_page_lvgl->p_ui_page_lvgl) - 16,
      lv_page_get_fit_height(gp_scrollabe_page_lvgl->p_ui_page_lvgl));
  lv_label_set_text(gp_scrollabe_page_lvgl->p_ui_body_lvgl,
                    gp_scrollabe_page_data->p_ui_body);
  lv_label_set_align(gp_scrollabe_page_lvgl->p_ui_body_lvgl,
                     LV_LABEL_ALIGN_CENTER);
  lv_obj_align(gp_scrollabe_page_lvgl->p_ui_body_lvgl,
               gp_scrollabe_page_lvgl->p_ui_page_lvgl,
               LV_ALIGN_IN_TOP_MID,
               0,
               0);
  /* Set callback of gp_scrollabe_page_lvgl->p_ui_body_lvgl to
   * page_arrow_handler which handles the actual scrolling */
  lv_obj_set_event_cb(gp_scrollabe_page_lvgl->p_ui_body_lvgl,
                      page_arrow_handler);

  /**
   * Create a label on page gp_scrollabe_page_lvgl->p_ui_page_lvgl which
   * contains padding of dummy text (\n or \n\n character) of height
   * lv_page_get_fit_height(gp_scrollabe_page_lvgl->p_ui_page_lvgl) / 2
   * In worst case, this label will serve as padding for the scrolling page.
   *
   */
  lv_obj_t *paddingLabel =
      lv_label_create(gp_scrollabe_page_lvgl->p_ui_page_lvgl, NULL);
  lv_label_set_long_mode(paddingLabel, LV_LABEL_LONG_BREAK);
  lv_obj_set_size(
      paddingLabel,
      lv_page_get_fit_width(gp_scrollabe_page_lvgl->p_ui_page_lvgl) - 16,
      lv_page_get_fit_height(gp_scrollabe_page_lvgl->p_ui_page_lvgl) / 2);

  // Pad with \n if there are 2 rows of text and pad with \n\n if there are 3
  // rows of text
  if (32 == scroll_page_height &&
      !gp_scrollabe_page_data->bool_only_first_page_header_visible) {
    lv_label_set_text(paddingLabel, "\n");
  } else {
    lv_label_set_text(paddingLabel, "\n\n");
  }

  lv_label_set_align(paddingLabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(paddingLabel,
               gp_scrollabe_page_lvgl->p_ui_body_lvgl,
               LV_ALIGN_OUT_BOTTOM_MID,
               0,
               0);

  /**
   * Register lvgl label gp_scrollabe_page_lvgl->p_ui_body_lvgl to learn about
   * external events emitted by ui_get_indev() In case any event is generated,
   * it will be handled by callback set for
   * gp_scrollabe_page_lvgl->p_ui_body_lvgl
   */
  lv_group_add_obj(ui_get_group(), gp_scrollabe_page_lvgl->p_ui_body_lvgl);
  lv_indev_set_group(ui_get_indev(), ui_get_group());
  lv_group_focus_obj(gp_scrollabe_page_lvgl->p_ui_body_lvgl);

  /**
   * Create a labels on current screen to hold the left and right arrows
   * icons. These icons will be placed on the left and right side of the
   * screen. These icons will be visible conditionally (if there is anything
   * to scroll)
   */
  /* TODO: Handle vertical scrolling input by processing page_orientation
   * argument */
  gp_scrollabe_page_lvgl->p_ui_left_arrow_lvgl =
      lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(gp_scrollabe_page_lvgl->p_ui_left_arrow_lvgl,
                    LV_SYMBOL_LEFT);
  lv_obj_align(gp_scrollabe_page_lvgl->p_ui_left_arrow_lvgl,
               lv_scr_act(),
               LV_ALIGN_IN_LEFT_MID,
               0,
               0);

  gp_scrollabe_page_lvgl->p_ui_right_arrow_lvgl =
      lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(gp_scrollabe_page_lvgl->p_ui_right_arrow_lvgl,
                    LV_SYMBOL_RIGHT);
  lv_obj_align(gp_scrollabe_page_lvgl->p_ui_right_arrow_lvgl,
               lv_scr_act(),
               LV_ALIGN_IN_RIGHT_MID,
               0,
               0);

  /* gp_scrollabe_page_lvgl->ui_arrow_released_style: This style is default
   * design for each of the arrows */
  lv_style_copy(&(gp_scrollabe_page_lvgl->ui_arrow_released_style),
                &lv_style_plain);

  /* gp_scrollabe_page_lvgl->ui_arrow_pressed_style: This style will be drawn
   * on the arrow if scrolling icon is pressed */
  lv_style_copy(&(gp_scrollabe_page_lvgl->ui_arrow_pressed_style),
                &lv_style_plain);
  (gp_scrollabe_page_lvgl->ui_arrow_pressed_style).body.main_color =
      LV_COLOR_BLACK;
  (gp_scrollabe_page_lvgl->ui_arrow_pressed_style).body.grad_color =
      LV_COLOR_BLACK;
  (gp_scrollabe_page_lvgl->ui_arrow_pressed_style).body.radius = 30;
  (gp_scrollabe_page_lvgl->ui_arrow_pressed_style).text.color = LV_COLOR_WHITE;
  lv_label_set_body_draw(gp_scrollabe_page_lvgl->p_ui_left_arrow_lvgl, true);
  lv_label_set_body_draw(gp_scrollabe_page_lvgl->p_ui_right_arrow_lvgl, true);

  /**
   * Calculate the number of pages/max number of times full scrolling can take
   * place Total number of scrolls = Total height of content on page / Height
   * of page We have already padded the content on page with label
   * paddingLabel and therefore we do not need to consider padding here
   */
  int16_t totalPageHeight =
      (int16_t)lv_page_get_scrl_height(gp_scrollabe_page_lvgl->p_ui_page_lvgl);
  int16_t currPageHeight =
      (int16_t)lv_obj_get_height(gp_scrollabe_page_lvgl->p_ui_page_lvgl);
  ASSERT(0 != currPageHeight);
  gp_scrollabe_page_data->curr_page_num = 1;
  gp_scrollabe_page_data->total_page_num = totalPageHeight / currPageHeight;

  /**
   * Recalculate the total pages if heading is only visible on the first page
   */
  if (gp_scrollabe_page_data->bool_only_first_page_header_visible) {
    gp_scrollabe_page_data->total_page_num = 1;
    int16_t first_page_height = currPageHeight;

    gp_scrollabe_page_data->curr_page_num = 2;
    page_update_header();

    int16_t curr_page_height =
        (int16_t)lv_obj_get_height(gp_scrollabe_page_lvgl->p_ui_page_lvgl);
    gp_scrollabe_page_data->total_page_num +=
        (totalPageHeight - first_page_height) / curr_page_height;

    gp_scrollabe_page_data->curr_page_num = 1;
    page_update_header();
  }

  /**
   * Create buttons on the screen for cancellation and confirmation.
   * These buttons will be visible conditionally (if the current page is the
   * last page)
   */
  gp_scrollabe_page_lvgl->p_ui_cancel_btn_lvgl =
      lv_btn_create(lv_scr_act(), NULL);
  ui_cancel_btn(gp_scrollabe_page_lvgl->p_ui_cancel_btn_lvgl,
                page_cancel_handler,
                gp_scrollabe_page_data->bool_accept_cancel_hidden);
  lv_obj_set_size(gp_scrollabe_page_lvgl->p_ui_cancel_btn_lvgl, 16, 16);

  gp_scrollabe_page_lvgl->p_ui_accept_btn_lvgl =
      lv_btn_create(lv_scr_act(), NULL);
  ui_next_btn(gp_scrollabe_page_lvgl->p_ui_accept_btn_lvgl,
              page_accept_handler,
              gp_scrollabe_page_data->bool_accept_cancel_hidden);
  lv_obj_set_size(gp_scrollabe_page_lvgl->p_ui_accept_btn_lvgl, 16, 16);

  /**
   * Create a label gp_scrollabe_page_lvgl->p_ui_footnote_lvgl which holds the
   * text that goes as part of a footnote on the current screen
   * Footnote to be shown only if total pages > 1
   */
  gp_scrollabe_page_lvgl->p_ui_footnote_lvgl = NULL;
  if (1 < gp_scrollabe_page_data->total_page_num) {
    gp_scrollabe_page_lvgl->p_ui_footnote_lvgl =
        lv_label_create(lv_scr_act(), NULL);
  }

  /* Update all icons: Left/right arrows, Accept/Cancel buttons and Footnote
   */
  page_update_icons();

  return;
}

void ui_scrollable_page(const char *p_page_ui_heading,
                        const char *p_page_ui_body,
                        e_scrollable_page_orientation_t page_orientation,
                        bool bool_cancel_accept_btn_visible) {
  if (NULL == p_page_ui_body) {
    return;
  }

  lv_obj_clean(lv_scr_act());

  gp_scrollabe_page_data =
      (scrolling_page_data_t *)malloc(sizeof(scrolling_page_data_t));
  ASSERT(NULL != gp_scrollabe_page_data);

  gp_scrollabe_page_data->p_ui_heading = p_page_ui_heading;
  gp_scrollabe_page_data->p_ui_body = p_page_ui_body;
  gp_scrollabe_page_data->bool_accept_cancel_visible =
      bool_cancel_accept_btn_visible;

  /* Below fields will be overwritten below, when page settings are being
   * applied */
  gp_scrollabe_page_data->total_page_num = 1;
  gp_scrollabe_page_data->curr_page_num = 1;
  gp_scrollabe_page_data->bool_left_arrow_hidden = true;
  gp_scrollabe_page_data->bool_right_arrow_hidden = true;
  gp_scrollabe_page_data->bool_accept_cancel_hidden = false;

  ui_scrollable_page_create();

#ifdef DEV_BUILD
  ekp_enqueue(LV_KEY_UP, DEFAULT_DELAY);
  for (int i = 0; i < gp_scrollabe_page_data->total_page_num; i++)
    ekp_enqueue(LV_KEY_RIGHT, DEFAULT_DELAY);
  // ekp_enqueue(LV_KEY_DOWN,DEFAULT_DELAY);
  ekp_enqueue(LV_KEY_ENTER, DEFAULT_DELAY);
#endif

  return;
}

void ui_scrollable_page_with_options(const char *p_page_ui_heading,
                                     const char *p_page_ui_body,
                                     scrollable_page_options_t options) {
  if (NULL == p_page_ui_body) {
    return;
  }

  lv_obj_clean(lv_scr_act());

  gp_scrollabe_page_data =
      (scrolling_page_data_t *)malloc(sizeof(scrolling_page_data_t));
  ASSERT(NULL != gp_scrollabe_page_data);

  gp_scrollabe_page_data->p_ui_heading = p_page_ui_heading;
  gp_scrollabe_page_data->p_ui_body = p_page_ui_body;
  gp_scrollabe_page_data->bool_accept_cancel_visible =
      options.are_cancel_accept_btn_visible;
  gp_scrollabe_page_data->bool_only_first_page_header_visible =
      !options.is_heading_sticky;

  /* Below fields will be overwritten below, when page settings are being
   * applied */
  gp_scrollabe_page_data->total_page_num = 1;
  gp_scrollabe_page_data->curr_page_num = 1;
  gp_scrollabe_page_data->bool_left_arrow_hidden = true;
  gp_scrollabe_page_data->bool_right_arrow_hidden = true;
  gp_scrollabe_page_data->bool_accept_cancel_hidden = false;

  ui_scrollable_page_create();

#ifdef DEV_BUILD
  ekp_enqueue(LV_KEY_UP, DEFAULT_DELAY);
  for (int i = 0; i < gp_scrollabe_page_data->total_page_num; i++)
    ekp_enqueue(LV_KEY_RIGHT, DEFAULT_DELAY);
  // ekp_enqueue(LV_KEY_DOWN,DEFAULT_DELAY);
  ekp_enqueue(LV_KEY_ENTER, DEFAULT_DELAY);
#endif

  return;
}
