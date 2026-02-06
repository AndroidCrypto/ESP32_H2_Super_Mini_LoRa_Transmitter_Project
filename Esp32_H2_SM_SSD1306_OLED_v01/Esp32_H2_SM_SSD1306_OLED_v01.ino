/*
  This sketch is explained in my tutorial "",
  published here:
  It shows the usage of an external SSD1306 OLED display with a resolution of 128x64 pixels.

  In Arduino, use ESP32H2 Dev Module for compiling and uploading.
  Don't forget to enable the option "USB CDC on boot enabled" in Tools menue
  to see output on Serial Monitor.
*/

/*
  Version Management
05.02.2026 V01 Initial programming  
*/

#include "SSD1306_OLED.h"

char* PROGRAM_VERSION = "ESP32 H2 Super Mini External SSD1306 OLED display V01";

//#define TEST_48MHZ_CPU_FREQUENCY

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(PROGRAM_VERSION);

#ifdef TEST_48MHZ_CPU_FREQUENCY
  // test that the sensor is working on 48 MHz CPU frequency as well
  Serial.printf("CPU frequency before setting: %d MHz\n", getCpuFrequencyMhz());
  setCpuFrequencyMhz(48);
  Serial.printf("CPU frequency after  setting: %d MHz\n", getCpuFrequencyMhz());
#else
  Serial.printf("CPU frequency: %d MHz\n", getCpuFrequencyMhz());
#endif

  setupSsd1306();
  oDisplay1 = "The OLED display";
  oDisplay2 = "SSD1306 128x64 px";
  oDisplay3 = "is ready to use";
  oDisplayData();
  delay(2000);

}

void loop() {

  oDisplayTestdata9();
  delay(2000);
  oDisplayTestdata13();
  delay(2000);
  oDisplayTestdata18();
  delay(2000);
  oDisplayTestdata1813();
  delay(2000);
  oDisplayClearData();
  oDisplayData();
  delay(2000);
  oDisplayInvertDisplay();
  delay(2000);
  oDisplayPowerOff();
  delay(2000);

}
