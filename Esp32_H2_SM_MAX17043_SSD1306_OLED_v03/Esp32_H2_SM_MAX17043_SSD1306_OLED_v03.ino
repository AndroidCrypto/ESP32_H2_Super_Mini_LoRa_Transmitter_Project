/*
  This sketch is explained in my tutorial "",
  published here:
  It shows the usage of an external MAX17043 Battery Voltage meter.
  To get values when powered by a battery, the SSD1306 OLED helper is included
  (see separate tutorial for more information about this helper file).

  In Arduino, use ESP32H2 Dev Module for compiling and uploading.
  Don't forget to enable the option "USB CDC on boot enabled" in Tools menue
  to see output on Serial Monitor.
*/

/*
  Version Management
11.02.2026 V03 Moving all battery related variables and methods to 'BATTERY_MANAGEMENT',
               this helper is for MAX17043 related data only  
07.02.2026 V02 Renaming of some methods  
06.02.2026 V01 Initial programming
*/

#include "MAX17043_METER.h"
#include "SSD1306_OLED.h"

char* PROGRAM_VERSION = "ESP32 H2 Super Mini External MAX17043 Battery Voltage Meter and SSD1306 OLED display V03";

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

  setupMax17043();

  setupSsd1306();
  oDisplay1 = "The OLED display";
  oDisplay2 = "SSD1306 128x64 px";
  oDisplay3 = "and MAX17043 meter";
  oDisplay4 = "are ready to use";
  oDisplayData();
  delay(2000);

}

void loop() {
  counter++;
  // wake up the meter
  if (isMax17043Sleeping()) {
    setMax17043Wake();
  }
  getMax17043Values();
  printMax17043Values();
  oDisplay1 = "Bat:" + String(batteryVoltage, 2) + " V";
  oDisplay2 = "Capacity: " + String(batteryCapacity) + " %";
  oDisplay3 = isMax17043Sleeping() ? "is sleeping: Yes" : "is sleeping: No";
  oDisplay4 = "MAX17043 " + String(counter) + " W";
  oDisplayData1813();
  delay(2000);
  setMax17043Sleep();
  // to show the difference
  printMax17043Values();
  oDisplay1 = "Bat:" + String(batteryVoltage, 2) + " V";
  oDisplay2 = "Capacity: " + String(batteryCapacity) + " %";
  oDisplay3 = isMax17043Sleeping() ? "is sleeping: Yes" : "is sleeping: No";
  oDisplay4 = "MAX17043 " + String(counter) + " S";
  oDisplayData1813();
  delay(2000);
}

/*
ESP32 H2 Super Mini External MAX17043 Battery Voltage Meter and SSD1306 OLED display V01
CPU frequency before setting: 96 MHz
CPU frequency after  setting: 48 MHz
Wire.begin was success
Initializing the MAX17043 instance without an address
MAX1704 set address to 36
The MAX17043 Fuel Gauge is set up
The SSD1306 OLED display is set up
ADC: 3375 Voltage: 4.22 V Remain Capacity: 99 %
Address: 0x36, Version: 3, compensation 0x97
isSleeping No alert active: No
ADC: 3375 Voltage: 4.22 V Remain Capacity: 99 %
Address: 0x36, Version: 3, compensation 0x97
isSleeping Yes alert active: No
*/