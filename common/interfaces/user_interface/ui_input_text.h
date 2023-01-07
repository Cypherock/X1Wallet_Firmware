/**
 * @file    ui_input_text.h
 * @author  Cypherock X1 Team
 * @brief   Title of the file.
 *          Short description of the file
 * @copyright Copyright (c) 2022 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/" target=_blank>https://mitcc.org/</a>
 * 
 */
#ifndef UI_INPUT_TEXT_H
#define UI_INPUT_TEXT_H

#include "ui_common.h"
#include "crypto_random.h"

#define MAX_INPUT_SIZE 10 //This is the max number of characters that can be shown in a screen
#define MAX_CHARACTER_INPUT_LIST 80
#define MAX_PIN_SIZE 8
#define MAX_PASSPHRASE_SIZE 63
#define MAX_ARRAY_SIZE 512

typedef enum {
    DATA_TYPE_TEXT,
    DATA_TYPE_PASSPHRASE,
    DATA_TYPE_PIN
} INPUT_DATA_TYPE;

/**
 * @brief struct for Input text data
 * @details
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
struct Input_Text_Data {
	const char *input_list;
	uint8_t input_list_size;
	char *initial_heading;
	int current_index;
	int current_display_index;
	char current_text[12];
	char entered_text[MAX_ARRAY_SIZE];
	char password_text[32];
	char display_entered_text[32];
	uint8_t min_input_size;
	uint8_t max_input_size;
	INPUT_DATA_TYPE data_type;
};

/**
 * @brief struct for managing components of input text component
 * @details
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
struct Input_Text_Object {
	lv_obj_t *text_entered;
	lv_obj_t *character;
	lv_obj_t *left_arrow;
	lv_obj_t *right_arrow;
	lv_obj_t *backspace;
	lv_obj_t *cancel_btn;
	lv_obj_t *next_btn;
};

/**
 * @brief Initialize and create input text UI
 * @details
 * 
 * @param input_list Input list of characters
 * @param initial_heading Input heading text
 * @param min_input_size Mininum input text size
 * @param data_type data type PASSWORD, PASSPHRASE or TEXT
 * @param max_input_size Maximum input text size
 *
 * @return
 * @retval
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
void input_text_init(const char *input_list, const char *initial_heading, uint8_t min_input_size, INPUT_DATA_TYPE data_type ,uint8_t max_input_size);

/**
 * @brief Create input text UI
 * @details
 *
 * @param
 *
 * @return
 * @retval
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
void input_text_create();

#endif // !UI_INPUT_TEXT_H
