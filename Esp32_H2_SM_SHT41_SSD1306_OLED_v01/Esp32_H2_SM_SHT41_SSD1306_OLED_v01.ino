/*
  This sketch is explained in my tutorial 
  "Use an SHT41 Environment Sensor with an ESP32-H2 Super Mini Development Board and get indoor temperature and humidity",
  It shows the usage of an external SHT41 environment sensor for temperature and humditiy.
  To get values when powered by a battery, the SSD1306 OLED helper is included
  (see separate tutorial for more information about this helper file).

  In Arduino, use ESP32H2 Dev Module for compiling and uploading.
  Don't forget to enable the option "USB CDC on boot enabled" in Tools menue
  to see output on Serial Monitor.
*/

/*
  Version Management
06.02.2026 V01 Initial programming  
*/

#include "SHT41_SENSOR.h"
#include "SSD1306_OLED.h"

char* PROGRAM_VERSION = "ESP32 H2 Super Mini External SHT41 Environment Sensor and SSD1306 OLED display V01";

#define TEST_48MHZ_CPU_FREQUENCY

uint32_t counter = 0;

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

  setupSsd1306();
  oDisplay1 = "The OLED display";
  oDisplay2 = "SSD1306 128x64 px";
  oDisplay3 = "and Sensor SHT41";
  oDisplay4 = "are ready to use";
  oDisplayData();
  delay(2000);
}

void loop() {

  counter++;
  getSht41Values();
  printSht41Values();
  oDisplay1 = "Temp:" + String(sht41_temperature, 1) + " C";
  oDisplay2 = "Humid: " + String(sht41_humidity, 0) + " %rH";
  oDisplay3 = "Data from SHT41";
  oDisplay4 = "Counter: " + String(counter);
  oDisplayData1813();
  delay(2000);
}

/*
ESP32 H2 Super Mini External SHT41 Environment Sensor and SSD1306 OLED display V01
CPU frequency before setting: 96 MHz
CPU frequency after  setting: 48 MHz
Wire1.begin was success
Init SHT41 SUCC
Found SHT4x sensor
Serial number 0x113D39BE
High precision
No heater
The SHT41 sensor is set up
The SSD1306 OLED display is set up
SHT41 Temperature: 21.49 C Humidity: 45.8 %rH Pressure: n.a. Altitude: n/a
SHT41 Temperature: 21.49 C Humidity: 45.8 %rH Pressure: n.a. Altitude: n/a
*/
