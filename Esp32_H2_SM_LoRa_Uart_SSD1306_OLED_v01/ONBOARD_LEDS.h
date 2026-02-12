/*
  Management of the onboard LED and RGB LED
  05.02.2026 V02 Implemented color lookup helper for RGB LED
  04.02.2026 V01 Initial version
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

void setupOnboardLeds() {
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  digitalWrite(ONBOARD_LED_PIN, LOW);  // LED off
  FastLED.addLeds<WS2812, ONBOARD_RGB_LED_PIN, GRB>(rgbLed, NUM_RGB_LEDS);
  rgbLed[0] = CRGB::Black; // RGB LED off
  FastLED.show();
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

/*
Temperature (Celsius) - RGB LED color
< 10      - white
10...< 19  - blue
19...< 22  - green
22...< 24  - yellow
>= 24      - red
*/
void onboardRgbLedTemperatureLookUp(float temperature) {
  if (temperature < 10) {
    setRgbLedWhite(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  } else if (temperature < 19) {
    setRgbLedBlue(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  } else if (temperature < 22) {
    setRgbLedGreen(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  } else if (temperature < 24) {
    setRgbLedYellow(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  } else {
    setRgbLedRed(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  }
}

/*
Battery Voltage (Volts) - RGB LED color
< 3.4       - red
3.4...< 3.5 - yellow
3.5...< 3.7 - green
3.7...< 4.1 - mint
>= 4.1      - blue
*/
void onboardRgbLedBatteryVoltageLookUp(float voltage) {
  if (voltage < 3.4) {
    setRgbLedRed(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  } else if(voltage < 3.5) {
    setRgbLedYellow(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  } else if(voltage < 3.7) {
    setRgbLedGreen(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  } else if(voltage < 4.1) {
    setRgbLedMint(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  } else {
    setRgbLedBlue(ONBOARD_RGB_LED_DEFAULT_BRIGHTNESS);
    return;
  }
}
