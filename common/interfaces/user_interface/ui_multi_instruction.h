/**
 * @file    ui_multi_instruction.h
 * @author  Cypherock X1 Team
 * @brief   Title of the file.
 *          Short description of the file
 * @copyright Copyright (c) 2022 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/" target=_blank>https://mitcc.org/</a>
 * 
 */
#ifndef UI_MULTI_INSTRUCTION_H
#define UI_MULTI_INSTRUCTION_H

#include "ui_common.h"

/// Maximum number of instruction screens
#define MAX_NUM_OF_INSTRUCTIONS 7

/// Maximum number of character in a single instruction screen
#define MAX_NUM_OF_CHARS_IN_AN_INSTRUCTION 85

/**
 * @brief Struct to hold state of the instruction screen
 * @details
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
struct Multi_Instruction_Data {
  char strings[MAX_NUM_OF_INSTRUCTIONS][MAX_NUM_OF_CHARS_IN_AN_INSTRUCTION];
  bool
      one_cycle_completed;  //if all the text has been shown once then this variable will be true else false
  bool
      destruct_on_click;  // if destruct_on_click == true the screen will destroy on a button click
  uint8_t total_strings;
  uint8_t index_of_current_string;
};

/**
 * @brief Struct to store the lvgl objects on the screen
 * @details
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
struct Multi_Instruction_Object {
  lv_obj_t *text;
  lv_obj_t *left_arrow;
  lv_obj_t *right_arrow;
};

/**
 * @brief Creates a slideshow of instruction screen with manual left and right controll arrows
 * @details
 * 
 * @param arr Array of strings to display on screen
 * @param count Number of strings (should be less than MAX_NUM_OF_INSTRUCTIONS)
 * @param delay_in_ms Time in milliseconds after which next screen will be shown
 * @param destruct_on_click Bool to allow clear screen on click
 *
 * @return
 * @retval
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
void multi_instruction_init(const char **arr,
                            uint8_t count,
                            uint16_t delay_in_ms,
                            bool destruct_on_click);

#endif  // UI_MULTI_INSTRUCTION_H