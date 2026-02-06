/*
  This sketch is explained in my tutorial "",
  published here:
  It shows the usage of the onboard LED and RGB LED by some helper methods.

  In Arduino, use ESP32H2 Dev Module for compiling and uploading.
  Don't forget to enable the option "USB CDC on boot enabled" in Tools menue
  to see output on Serial Monitor.
*/

/*
  Version Management
05.02.2026 V02 Implemented color lookup helper for RGB LED, added test for low CPU frequency
04.02.2026 V01 Initial programming  
*/

#include "ONBOARD_LEDS.h"

char* PROGRAM_VERSION = "ESP32 H2 Super Mini Onboard LED and RGB LED V02";

#define TEST_48MHZ_CPU_FREQUENCY

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(PROGRAM_VERSION);

  #ifdef TEST_48MHZ_CPU_FREQUENCY
  // test that the sensor is working on 48 MHz CPU frequency as well
  Serial.printf("CPU frequency before setting: %d MHz\n", getCpuFrequencyMhz());
  setCpuFrequencyMhz(48);
  Serial.printf("CPU frequency after  setting: %d MHz\n", getCpuFrequencyMhz());
#else
  Serial.printf("CPU frequency: %d MHz\n", getCpuFrequencyMhz());
#endif

  setupOnboardLeds();
  Serial.println("The setup of the onboard LED and RGB LED is done");

}

void loop() {
  Serial.println("The onboard LED is ON");
  setOnboardLed(true);
  delay(1000);
  Serial.println("The onboard LED is OFF");
  setOnboardLed(false);
  delay(1000);
  Serial.println("The onboard LED is Blinking 2 times");
  onboardLedFlash(2, 125);
  delay(1000);
  Serial.println("The onboard RGB LED is RED lightning");
  setRgbLedRed(10);
  delay(1000);
  Serial.println("The onboard RGB LED is GREEN lightning");
  setRgbLedGreen(10);
  delay(1000);
  Serial.println("The onboard RGB LED is BLUE lightning");
  setRgbLedBlue(10);
  delay(1000);
Serial.println("The onboard RGB LED is YELLOW lightning");
  setRgbLedYellow(10);
  delay(1000);
Serial.println("The onboard RGB LED is MINT lightning");
  setRgbLedMint(10);
  delay(1000);
Serial.println("The onboard RGB LED is PURPLE lightning");
  setRgbLedPurple(10);
  delay(1000);
  Serial.println("The onboard RGB LED is WHITE lightning");
  setRgbLedWhite(10);
  delay(1000);
  Serial.println("The onboard RGB LED is OFF");
  setRgbLedOff();
  delay(1000);
  Serial.println("The onboard RGB LED is Blinking 2 times in RED");
  onboardLedFlashRed(2, 125);
  delay(1000);
  Serial.println("The onboard RGB LED is Blinking 2 times in GREEN");
  onboardLedFlashGreen(2, 125);
  delay(1000);
  Serial.println("The onboard RGB LED is Blinking 2 times in BLUE");
  onboardLedFlashBlue(2, 125);
  delay(1000);
  Serial.println("The onboard RGB LED is Blinking 2 times in WHITE");
  onboardLedFlashWhite(2, 125);
  
  // temperature lookup
  float temperature = 9.8;
  Serial.printf("Temperature look up for %f Celsius\n", temperature);
  onboardRgbLedTemperatureLookUp(temperature);
  delay(1000);
  temperature = 14.3;
  Serial.printf("Temperature look up for %f Celsius\n", temperature);
  onboardRgbLedTemperatureLookUp(temperature);
  delay(1000);
  temperature = 21.1;
  Serial.printf("Temperature look up for %f Celsius\n", temperature);
  onboardRgbLedTemperatureLookUp(temperature);
  delay(1000);
  temperature = 21.1;
  Serial.printf("Temperature look up for %f Celsius\n", temperature);
  onboardRgbLedTemperatureLookUp(temperature);
  delay(1000);
  temperature = 23.5;
  Serial.printf("Temperature look up for %f Celsius\n", temperature);
  onboardRgbLedTemperatureLookUp(temperature);
  delay(1000);
  temperature = 25.2;
  Serial.printf("Temperature look up for %f Celsius\n", temperature);
  onboardRgbLedTemperatureLookUp(temperature);
  delay(1000);
  Serial.println("The onboard RGB LED is OFF");
  setRgbLedOff();
  delay(1000);
  
  // battery voltage lookup
  float batteryVoltage = 3.1;
  Serial.printf("Battery voltage look up for %f volts\n", batteryVoltage);
  onboardRgbLedBatteryVoltageLookUp(batteryVoltage);
  delay(1000);
  batteryVoltage = 3.45;
  Serial.printf("Battery voltage look up for %f volts\n", batteryVoltage);
  onboardRgbLedBatteryVoltageLookUp(batteryVoltage);
  delay(1000);
  batteryVoltage = 3.56;
  Serial.printf("Battery voltage look up for %f volts\n", batteryVoltage);
  onboardRgbLedBatteryVoltageLookUp(batteryVoltage);
  delay(1000);
  batteryVoltage = 3.72;
  Serial.printf("Battery voltage look up for %f volts\n", batteryVoltage);
  onboardRgbLedBatteryVoltageLookUp(batteryVoltage);
  delay(1000);
  batteryVoltage = 4.13;
  Serial.printf("Battery voltage look up for %f volts\n", batteryVoltage);
  onboardRgbLedBatteryVoltageLookUp(batteryVoltage);
  delay(1000);
  Serial.println("The onboard RGB LED is OFF");
  setRgbLedOff();
  delay(1000);

  Serial.println("The onboard RGB LED is WHITE dimming to 255 and back to 0");
  for (uint8_t i = 0; i < 255; i++) {
    setRgbLedWhite(i + 1);
    delay(20);
  }
  for (uint8_t i = 0; i < 255; i++) {
    setRgbLedWhite(255 - (i + 1));
    delay(20);
  }
  
  Serial.println("Wait for 5 seconds for next round");
  delay(5000);

}
