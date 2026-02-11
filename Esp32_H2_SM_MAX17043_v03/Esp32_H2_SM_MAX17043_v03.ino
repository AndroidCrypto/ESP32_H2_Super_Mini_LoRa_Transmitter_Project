/*
  This sketch is explained in my tutorial 
  "Use a MAX17043 battery voltage meter with an ESP32-H2 Super Mini Development Board 
   and get the battery voltage of your lithium battery and its remaining capacity"

  It shows the usage of an external MAX17043 Battery Voltage meter.

  In Arduino, use ESP32H2 Dev Module for compiling and uploading.
  Don't forget to enable the option "USB CDC on boot enabled" in Tools menue
  to see output on Serial Monitor.
*/

/*
  Version Management
08.02.2026 V03 Moving all battery related variables and methods to 'BATTERY_MANAGEMENT',
               this helper is for MAX17043 related data only
07.02.2026 V02 Renaming of some methods  
05.02.2026 V01 Initial programming  
*/

#include "MAX17043_METER.h"

char* PROGRAM_VERSION = "ESP32 H2 Super Mini External MAX17043 Battery Voltage Meter V03";

#define TEST_48MHZ_CPU_FREQUENCY

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

}

void loop() {
  // wake up the meter
  if (isMax17043Sleeping()) {
    setMax17043Wake();
  }
  getMax17043Values();
  printMax17043Values();
  delay(2000);
  setMax17043Sleep();
  // to show the difference
  printMax17043Values();
  delay(2000);
}

/*
ADC: 3344 Voltage: 4.18 V Remain Capacity: 98 %
Address: 0x36, Version: 3, compensation 0x97
isSleeping No alert active: No
*/
