/*
  Management of an external MAX17043 Battery Voltage meter
  08.02.2026 V03 Moving all battery related variables and methods to 'BATTERY_MANAGEMENT',
                 this helper is for MAX17043 related data only.
  07.02.2026 V02 Renaming of some methods
  05.02.2026 V01 Initial version
  
*/

#ifndef IS_MAX17043_METER
#define IS_MAX17043_METER
#endif

#include "BATTERY_MANAGEMENT.h"

// ------------------------------------------------------------------
// MAX17043 meter

#define MAX17043_I2C_SDA_PIN 5
#define MAX17043_I2C_SCL_PIN 4
#define MAX17043_I2C_ADDRESS 0x36  // from library

#include "MAX1704X.h"  // https://github.com/porrey/max1704x version 1.2.9

//
// Define the MAX17403 fuel gauge. Use MAX17044_mV
// for the MAX17044.
//
MAX1704X max17043 = MAX1704X(MAX17043_mV);

void setupMax17043(bool skipWire = false) {
  if (skipWire) {
    Serial.println("Setting of the I2C pins for Wire skipped");
  } else {
    // As both sensors are on the same Wire1 bus, we initialize the bus just with one I2C pin settings
    bool wireBegin = Wire.begin(MAX17043_I2C_SDA_PIN, MAX17043_I2C_SCL_PIN);
    if (wireBegin) {
      Serial.println("Wire.begin was success");
    } else {
      Serial.println("Wire.begin was FAILURE");
    }
  }
  //
  // Initialize the fuel gauge without an address.
  //
  Serial.println("Initializing the MAX17043 instance without an address");
  max17043.begin(&Wire, DEFER_ADDRESS);
  // now add the address
  max17043.address(MAX17043_I2C_ADDRESS);
  Serial.printf("MAX1704 set address to %02X\n", MAX17043_I2C_ADDRESS);
  max17043.reset();
  delay(250);
  max17043.quickstart();
  delay(125);
  Serial.println(F("The MAX17043 Fuel Gauge is set up"));
}

void setMax17043Sleep() {
  max17043.sleep();
}

void setMax17043Wake() {
  max17043.wake();
}

bool isMax17043Sleeping() {
  return max17043.isSleeping();
}

void getMax17043Values() {
  batteryVoltage = max17043.voltage() / 1000;
  calculateRemainingBatteryCapacity();
  checkVoltageThresholds();  // check for upper- and undervoltage
}

void printMax17043Values() {
  Serial.printf("ADC: %d Voltage: %2.2f V Remain Capacity: %d %%\n", max17043.adc(), batteryVoltage, batteryCapacity);
  Serial.printf("Address: 0x%2X, Version: %d, compensation 0x%2X\n", max17043.address(), max17043.version(), max17043.compensation());
  Serial.printf("isSleeping %s alert active: %s\n", max17043.isSleeping() ? "Yes" : "No", max17043.alertIsActive() ? "Yes" : "No");
  Serial.flush();
}

/*
// Code is taken from the "Inofficial Heltec V3" library by rogp (Rop Gonggrijp)
// https://github.com/ropg/heltec_esp32_lora_v3
// Don't you just hate it when battery percentages are wrong?
//
// I measured the actual voltage drop on a LiPo battery and these are the
// average voltages, expressed in 1/256'th steps between min_voltage and
// max_voltage for each 1/100 of the time it took to discharge the battery. The
// code for a telnet server that outputs battery voltage as CSV data is in
// examples, and a python script that outputs the constants below is in
// src/tools.
const float min_voltage = 3.04;
const float max_voltage = 4.26;
const uint8_t scaled_voltage[100] = {
  254,
  242,
  230,
  227,
  223,
  219,
  215,
  213,
  210,
  207,
  206,
  202,
  202,
  200,
  200,
  199,
  198,
  198,
  196,
  196,
  195,
  195,
  194,
  192,
  191,
  188,
  187,
  185,
  185,
  185,
  183,
  182,
  180,
  179,
  178,
  175,
  175,
  174,
  172,
  171,
  170,
  169,
  168,
  166,
  166,
  165,
  165,
  164,
  161,
  161,
  159,
  158,
  158,
  157,
  156,
  155,
  151,
  148,
  147,
  145,
  143,
  142,
  140,
  140,
  136,
  132,
  130,
  130,
  129,
  126,
  125,
  124,
  121,
  120,
  118,
  116,
  115,
  114,
  112,
  112,
  110,
  110,
  108,
  106,
  106,
  104,
  102,
  101,
  99,
  97,
  94,
  90,
  81,
  80,
  76,
  73,
  66,
  52,
  32,
  7,
};

int16_t calculateRemainingBatteryCapacity() {
  for (int n = 0; n < sizeof(scaled_voltage); n++) {
    float step = (max_voltage - min_voltage) / 256;
    if (batteryVoltage > min_voltage + (step * scaled_voltage[n])) {
      batteryCapacity = 100 - n;
      return batteryCapacity;
    }
  }
}
*/