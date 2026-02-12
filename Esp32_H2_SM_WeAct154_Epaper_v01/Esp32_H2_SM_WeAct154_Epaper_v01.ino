/*
  This sketch is explained in my tutorial 
  "Commissioning of a 1.54-inch e-paper display from WeAct to an ESP32-H2 Super Mini Development Board"

  It shows the usage of an external WeAct 1.54-inches 200x200 px e-paper.
  Please note that a fullRefresh is forced after MAXIMUM_PARTIAL_REFRESHES.

  In Arduino, use ESP32H2 Dev Module for compiling and uploading.
  Don't forget to enable the option "USB CDC on boot enabled" in Tools menue
  to see output on Serial Monitor.
*/

/*
  Version Management
08.02.2026 V01 Initial programming  
*/

// some dummy variables
float sht41_temperature = 0;
float sht41_humidity = 0;
uint32_t txCounter = 0;
float batteryVoltage = 0;
uint8_t batteryCapacity = 0;
uint8_t batteryUndervoltageDetectorCount = 0;
char budc[8]; // batteryUndervoltageDetectorCount as char
const uint8_t BATTERY_UNDERVOLTAGE_MAXIMUM_COUNTS = 3; // allows for maximal 3 undervoltage detection, then is goes in permanent deep sleep

#include "WEACT_154_EPAPER.h"

char* PROGRAM_VERSION = "ESP32 H2 Super Mini External WeAct 1.54-inches e-paper display V01";

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

  setupEpaperDisplay();

  // print the frame without data
  if (initial) epd.drawPaged(drawFull, 0);
  epd.hibernate();

}

void loop() {

  txCounter++;
  // get random values
  sht41_temperature = random(1500, 3000) / 100;
  sht41_humidity = random(3000, 5000) / 100;
  batteryVoltage = random(310, 427) / 100;
  batteryCapacity = random(1, 100);
  sprintf(budc, " UDC:%1d", batteryUndervoltageDetectorCount);
  Serial.printf("Counter: %d Temperature: %2.1f C Humidity: %2.1f %%rH Battery voltage: %1.2f v Capacity: %d %% UndervoltCounter: %d\n", txCounter, sht41_temperature, sht41_humidity, batteryVoltage, batteryCapacity, batteryUndervoltageDetectorCount);
  
  checkFullRefresh();
  
  epd.drawPaged(drawPartial, 0);
  epd.hibernate();
  //epd.powerOff(); // might be the best solution for long lasting deep sleep

  batteryUndervoltageDetectorCount++;
  if (batteryUndervoltageDetectorCount > BATTERY_UNDERVOLTAGE_MAXIMUM_COUNTS) batteryUndervoltageDetectorCount = 0;
  delay(2000);
}

/*
ADC: 3344 Voltage: 4.18 V Remain Capacity: 98 %
Address: 0x36, Version: 3, compensation 0x97
isSleeping No alert active: No
*/
