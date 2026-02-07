/*
  Management of an external SHT41 Environment Sensor
  05.02.2026 V01 Initial version
  
  SHT41 Temperature: 20.33 C Humidity: 46.9 %rH Pressure: n.a. Altitude: n/a
*/

#ifndef IS_SHT41_SENSOR
#define IS_SHT41_SENSOR
#endif

/*
Wiring SHT41 sensor - ESP32-H2 SM
GND - GND
VCC - 3.3V
SCL - GPIO 4
SDA - GPIO 5
*/

// ------------------------------------------------------------------
// SHT41 sensor

#define SHT4X_I2C_SDA_PIN 5     // blue
#define SHT4X_I2C_SCL_PIN 4     // green
#define SHT4X_I2C_ADDRESS 0x44  // Set to 0x45 for alternate i2c addr

#include "Adafruit_SHT4x.h"  // https://github.com/adafruit/Adafruit_SHT4x version 1.0.5
// Adafruit BusIO, Adafruit Unified Sensor, Adafruit SH110X, Adafruit SSD1306 are required as well
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

float sht41_temperature = -99;
float sht41_humidity = -99;
float sht41_pressure = -99;  // not available for this sensor
float sht41_altitude = -99;  // not available for this sensor

void setupSht41(bool skipWire1 = false) {
  if (skipWire1) {
    Serial.println("Setting of the I2C pins for Wire1 skipped");
  } else {
  // As both sensors are on the same Wire1 bus, we initialize the bus just with one I2C pin settings
  //bool wire1Begin = Wire1.begin(SHT4X_I2C_SDA_PIN, SHT4X_I2C_SCL_PIN);
  bool wire1Begin = Wire.begin(SHT4X_I2C_SDA_PIN, SHT4X_I2C_SCL_PIN);
  if (wire1Begin) {
    Serial.println("Wire1.begin was success");
  } else {
    Serial.println("Wire1.begin was FAILURE");
  }
  }

  //if (sht4.begin(&Wire1)) {
    if (sht4.begin(&Wire)) {
    Serial.println("Init SHT41 SUCC");
  } else {
    Serial.println("Init SHT41 FAIL");
  }

  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);

  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  //sht4.setPrecision(SHT4X_LOW_PRECISION);
  switch (sht4.getPrecision()) {
    case SHT4X_HIGH_PRECISION:
      Serial.println("High precision");
      break;
    case SHT4X_MED_PRECISION:
      Serial.println("Med precision");
      break;
    case SHT4X_LOW_PRECISION:
      Serial.println("Low precision");
      break;
  }

  // You can have 6 different heater settings
  // higher heat and longer times uses more power
  // and reads will take longer too!
  sht4.setHeater(SHT4X_NO_HEATER);
  //sht4.setHeater(SHT4X_HIGH_HEATER_100MS);
  switch (sht4.getHeater()) {
    case SHT4X_NO_HEATER:
      Serial.println("No heater");
      break;
    case SHT4X_HIGH_HEATER_1S:
      Serial.println("High heat for 1 second");
      break;
    case SHT4X_HIGH_HEATER_100MS:
      Serial.println("High heat for 0.1 second");
      break;
    case SHT4X_MED_HEATER_1S:
      Serial.println("Medium heat for 1 second");
      break;
    case SHT4X_MED_HEATER_100MS:
      Serial.println("Medium heat for 0.1 second");
      break;
    case SHT4X_LOW_HEATER_1S:
      Serial.println("Low heat for 1 second");
      break;
    case SHT4X_LOW_HEATER_100MS:
      Serial.println("Low heat for 0.1 second");
      break;
  }
  Serial.println(F("The SHT41 sensor is set up"));
}

void getSht41Values() {
  sensors_event_t humidity, temp;
  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp);  // populate temp and humidity objects with fresh data
  sht41_temperature = temp.temperature;
  sht41_humidity = humidity.relative_humidity;
}

void printSht41Values() {
  Serial.printf("SHT41 Temperature: %2.2f C Humidity: %2.1f %%rH Pressure: n.a. Altitude: n/a\n", sht41_temperature, sht41_humidity);
  Serial.flush();
}