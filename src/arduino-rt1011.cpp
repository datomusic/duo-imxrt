#include "Arduino.h"
#include "pin_mux.h"
#define TEENSYDUINO 999
// #include "Audio.h"
#include "bs814a.h"

// GUItool: begin automatically generated code
// AudioSynthWaveformSine   sine1;          //xy=305,275
// AudioOutputMQS           mqs1;           //xy=479,272
// AudioConnection          patchCord1(sine1, 0, mqs1, 0);
// AudioConnection          patchCord2(sine1, 0, mqs1, 1);
// GUItool: end automatically generated code

// CRGB leds[19];

/*!
 * @brief Main function
 */
int main(void) {
  /* Board pin init */
  BOARD_ConfigMPU();
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();
  BOARD_InitNeopixels();
  init(); // Seeeduino init

  uint8_t color[] = {0xFF, 0x0 , 0x00};
  board_rgb_write(color);
  /* BS814A_begin(); */

#define PIN_MQS_R D7
#define PIN_AMP_MUTE PIN_A2
#define PIN_HP_ENABLE 23U
  /* pinMode(PIN_MQS_R, OUTPUT);     // AD_02, MQS pin, pin 58 */
  /* pinMode(PIN_AMP_MUTE, OUTPUT);  // AD_10, /AMP_MUTE pin 47 */
  /* pinMode(PIN_HP_ENABLE, OUTPUT); // AD_11, /HP_ENABLE pin 46 */
  /* digitalWrite(PIN_AMP_MUTE, LOW); */
  /* digitalWrite(PIN_HP_ENABLE, HIGH); */

  // AudioMemory(100);
  // sine1.amplitude(0.5F);
  // sine1.frequency(440.0F);

  while (1) {
    ++color[1];
    board_rgb_write(color);
    /* digitalWrite(PIN_MQS_R, HIGH); */
    delayMicroseconds(2020);
  }
}
