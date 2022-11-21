#include "Arduino.h"
#include "lib/board_init.h"
#include <FastLED.h>
#define COLOR_ORDER GRB

#define LED_TYPE SK6812
#define CORRECTION_SK6812 0xFFF1E0
#define CORRECTION_SK6805 0xFFD3E0
#define LED_WHITE CRGB(230,255,150)

#define SK6812_BRIGHTNESS 32
#define SK6805_BRIGHTNESS 140
const CRGB COLORS[] = {
  0x444444,
  0xFF0001,
  0x444444,
  0xFFDD00,
  0x444444,
  0x444444,
  0x11FF00,
  0x444444,
  0x0033DD,
  0x444444,
  0xFF00FF,
  0x444444,
  0x444444,
  0xFF2209,
  0x444444,
  0x99FF00,
  0x444444,
  0x444444,
  0x00EE22,
  0x444444,
  0x0099CC,
  0x444444,
  0xBB33BB,
  0x444444
};

int main(void) {
  board_init();

  #define PIXELPIN 34U
  #define NUM_LEDS 19
  CRGB leds[NUM_LEDS];
  FastLED.addLeds<SK6812, PIXELPIN, GRB>(leds, NUM_LEDS);
  
  FastLED.setBrightness(SK6805_BRIGHTNESS); 
  FastLED.setCorrection(CORRECTION_SK6805);

  FastLED.clear();
  FastLED.show();

  while (true) {
    
    for(int i = 0; i < NUM_LEDS; i++) {
      //analogWrite(ENV_LED,i*8);
      //analogWrite(FILTER_LED,i*8);
      //analogWrite(OSC_LED,i*8);

      leds[i] = COLORS[i%24];
      delay(20);
      FastLED.show();
    }
    delay(1000);
    FastLED.clear();
    FastLED.show();
    delay(500);
  }
}
