// Key matrix
/*
  Rows are set to input_pullup and cols are pulled low one by one
*/
#include "keypad.h"

#define PIN_ROW_1            GPIO_13
#define PIN_ROW_2            GPIO_12
#define PIN_ROW_3            GPIO_SD_04
#define PIN_ROW_4            GPIO_11

#define PIN_COL_1           GPIO_AD_09
#define PIN_COL_2           GPIO_AD_08
#define PIN_COL_3           GPIO_AD_07
#define PIN_COL_4           GPIO_AD_06
#define PIN_COL_5           GPIO_AD_05
#define PIN_COL_6           GPIO_AD_04

enum BUTTONS {
  DUMMY_KEY,
  KEYB_0,  KEYB_1,  KEYB_2,  KEYB_3,  KEYB_4,  KEYB_5,  KEYB_6,  KEYB_7,  KEYB_8,  KEYB_9,
  STEP_1,  STEP_2,  STEP_3,  STEP_4,  STEP_5,  STEP_6,  STEP_7, STEP_8,
  BTN_DOWN, BTN_UP,
  BTN_SEQ1, BTN_SEQ2,
  SEQ_START
};

const uint8_t powerbutton_col = 1;
const uint8_t powerbutton_row = 1;
const uint8_t ROWS = 4;
const uint8_t COLS = 6;

uint8_t col_pins[COLS] = {PIN_COL_1,PIN_COL_2,PIN_COL_3,PIN_COL_4,PIN_COL_5,PIN_COL_6};
uint8_t row_pins[ROWS] = {PIN_ROW_1, PIN_ROW_2, PIN_ROW_3, PIN_ROW_4};

char buttons[ROWS][COLS] = {
{ BTN_SEQ1,  STEP_8,    STEP_1,   BTN_SEQ2, STEP_7, STEP_6 },
{ DUMMY_KEY, SEQ_START, STEP_2,   STEP_3,   STEP_4, STEP_5 },
{ KEYB_0,    BTN_DOWN,  KEYB_2,   KEYB_1,   KEYB_4, KEYB_3 },
{ KEYB_6,    KEYB_5,    KEYB_8,   KEYB_7,   BTN_UP, KEYB_9 }
};


Keypad button_matrix = Keypad( makeKeymap(buttons), row_pins, col_pins, ROWS, COLS );

void button_matrix_init();
bool keys_scan_powerbutton();

void button_matrix_init() {
    button_matrix.setDebounceTime(15);
    button_matrix.setHoldTime(2000);
}

bool keys_scan_powerbutton() {
  bool r = false;

  pinMode(row_pins[powerbutton_row],INPUT_PULLUP);
  pinMode(col_pins[powerbutton_col],OUTPUT);
  digitalWrite(col_pins[powerbutton_col],LOW);
  r = (digitalRead(row_pins[powerbutton_row]) == LOW);

  return r;
}


