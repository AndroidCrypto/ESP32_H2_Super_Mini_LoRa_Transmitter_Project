/*
  This sketch is explained in my tutorial "",
  published here:
  It shows the usage of an external SHT41 environment sensor for temperature and humditiy.

  In Arduino, use ESP32H2 Dev Module for compiling and uploading.
  Don't forget to enable the option "USB CDC on boot enabled" in Tools menue
  to see output on Serial Monitor.
*/

/*
  Version Management
05.02.2026 V02 Changed in setup from Wire1 to Wire  
05.02.2026 V01 Initial programming  
*/

#include "SHT41_SENSOR.h"

char* PROGRAM_VERSION = "ESP32 H2 Super Mini External SHT41 Environment Sensor V01";

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

  setupSht41();

}

void loop() {
  getSht41Values();
  printSht41Values();
  delay(1000);
}
