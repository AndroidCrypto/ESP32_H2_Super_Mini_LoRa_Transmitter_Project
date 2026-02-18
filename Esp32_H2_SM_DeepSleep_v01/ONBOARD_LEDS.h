/*
  Management of the onboard LED and RGB LED
  Version 01
*/

#ifndef IS_ONBOARD_LEDS
#define IS_ONBOARD_LEDS
#endif

#define ONBOARD_LED_PIN 13     // this is a small blue LED
#define ONBOARD_RGB_LED_PIN 8  // this is a WS2812 RGB LED
#define ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS 10

#include <FastLED.h> // https://github.com/FastLED/FastLED version 3.10.3
#define NUM_RGB_LEDS 1
CRGB rgbLed[NUM_RGB_LEDS];

// section LED
void setRgbLedOff(); // forward declaration

void setupOnboardLeds() {
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  digitalWrite(ONBOARD_LED_PIN, LOW);  // LED off
  FastLED.addLeds<WS2812, ONBOARD_RGB_LED_PIN, GRB>(rgbLed, NUM_RGB_LEDS);
  setRgbLedOff();
}

void setOnboardLed(bool isLedOn) {
  if (isLedOn) {
    digitalWrite(ONBOARD_LED_PIN, HIGH);  // LED on
  } else {
    digitalWrite(ONBOARD_LED_PIN, LOW);  // LED off
  }
}

void onboardLedFlash(uint16_t flashes, uint16_t delaymS) {
  uint16_t index;
  for (index = 1; index <= flashes; index++) {
    digitalWrite(ONBOARD_LED_PIN, HIGH);
    delay(delaymS);
    digitalWrite(ONBOARD_LED_PIN, LOW);
    delay(delaymS);
  }
}

void ledFlash(uint16_t flashes, uint16_t delaymS) {
  onboardLedFlash(flashes, delaymS);
}

// section RGB LED

void setRgbLedRed(uint8_t brightness) {
  FastLED.setBrightness(brightness);
  rgbLed[0] = CRGB::Red; 
  FastLED.show();
}

void setRgbLedGreen(uint8_t brightness) {
  FastLED.setBrightness(brightness);
  rgbLed[0] = CRGB::Green; 
  FastLED.show();
}

void setRgbLedBlue(uint8_t brightness) {
  FastLED.setBrightness(brightness);
  rgbLed[0] = CRGB::Blue; 
  FastLED.show();
}

void setRgbLedYellow(uint8_t brightness) {
  FastLED.setBrightness(brightness);
  rgbLed[0] = CRGB::Yellow; 
  FastLED.show();
}

void setRgbLedMint(uint8_t brightness) {
  FastLED.setBrightness(brightness);
  rgbLed[0] = CRGB::Aqua; 
  FastLED.show();
}

void setRgbLedPurple(uint8_t brightness) {
  FastLED.setBrightness(brightness);
  rgbLed[0] = CRGB::Purple; 
  FastLED.show();
}

void setRgbLedWhite(uint8_t brightness) {
  FastLED.setBrightness(brightness);
  rgbLed[0] = CRGB::White; 
  FastLED.show();
}

void setRgbLedOff() {
  FastLED.setBrightness(0);
  rgbLed[0] = CRGB::Black;
  FastLED.show();
}

void onboardLedFlashRed(uint16_t flashes, uint16_t delaymS) {
  uint16_t index;
  for (index = 1; index <= flashes; index++) {
    setRgbLedRed(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    delay(delaymS);
    setRgbLedOff();
    delay(delaymS);
  }
}

void onboardLedFlashGreen(uint16_t flashes, uint16_t delaymS) {
  uint16_t index;
  for (index = 1; index <= flashes; index++) {
    setRgbLedGreen(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    delay(delaymS);
    setRgbLedOff();
    delay(delaymS);
  }
}

void onboardLedFlashBlue(uint16_t flashes, uint16_t delaymS) {
  uint16_t index;
  for (index = 1; index <= flashes; index++) {
    setRgbLedBlue(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    delay(delaymS);
    setRgbLedOff();
    delay(delaymS);
  }
}

void onboardLedFlashWhite(uint16_t flashes, uint16_t delaymS) {
  uint16_t index;
  for (index = 1; index <= flashes; index++) {
    setRgbLedWhite(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    delay(delaymS);
    setRgbLedOff();
    delay(delaymS);
  }
}

void temperatureLookUp(float temperature) {

}