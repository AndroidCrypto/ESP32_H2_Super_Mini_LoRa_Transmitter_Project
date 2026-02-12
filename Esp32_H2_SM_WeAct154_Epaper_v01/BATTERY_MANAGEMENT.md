/*
  This file takes all code for the battery voltage.
  Measurements: battery voltage in volts, battery capacity in percent
  Note: some boards require an additional activation of the measuring, 
        enabled by BATTERY_VOLTAGE_ADC_MEASURE_CONTROL_PIN
*/

// This is activated when defining BATTERY_VOLTAGE_ADC_PIN 1

// Readings
float batteryVoltage = 0.0;
int16_t batteryCapacity = 0;
const float BATTERY_VOLTAGE_DIVIDER = 238.7;
const float BATTERY_UPPERVOLTAGE_DETECTOR_THRESHOLD = 4.0;  // if the measured battery voltage is higher than the threshold the counter is reset
//const float BATTERY_UNDERVOLTAGE_DETECTOR_THRESHOLD = 2.9;  // if the measured battery voltage is lower than the threshold the counter is increased by 1
const float BATTERY_UNDERVOLTAGE_DETECTOR_THRESHOLD = 3.4;  // if the measured battery voltage is lower than the threshold the counter is increased by 1
const float BATTERY_UNDERVOLTAGE_MAXIMUM_COUNTS = 3; // allows for maximal 3 undervoltage detection, then is goes in permanent deep sleep
// these data remain in ram even after deep sleep
RTC_DATA_ATTR bool isUndervoltageDeepSleep;  // if true the device stays in deep sleep until a manual reset
RTC_DATA_ATTR uint8_t batteryUndervoltageDetectorCount;

void printBatteryVoltage() {
#ifdef INCLUDE_SERIAL_MONITOR
  Serial.printf("Battery voltage: %f remaining Battery capacity: %d Undervolt counter: %d\n", batteryVoltage, batteryCapacity, batteryUndervoltageDetectorCount);
  Serial.flush();
#endif
}

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

void disableBatteryVoltageReading() {
#ifdef BATTERY_VOLTAGE_ADC_MEASURE_CONTROL_PIN
  // pulled up, no need to drive it
  pinMode(BATTERY_VOLTAGE_ADC_MEASURE_CONTROL_PIN, INPUT);
#endif
#ifdef BATTERY_VOLTAGE_ADC_PIN
  pinMode(BATTERY_VOLTAGE_ADC_PIN, INPUT);
#endif
}

// read the battery voltage
int16_t runBatteryVoltageReading() {
#ifdef BATTERY_VOLTAGE_ADC_MEASURE_CONTROL_PIN
  pinMode(BATTERY_VOLTAGE_ADC_MEASURE_CONTROL_PIN, OUTPUT);
  digitalWrite(BATTERY_VOLTAGE_ADC_MEASURE_CONTROL_PIN, LOW);
  delay(5);
#endif
#ifdef BATTERY_VOLTAGE_ADC_PIN
  batteryVoltage = analogRead(BATTERY_VOLTAGE_ADC_PIN) / BATTERY_VOLTAGE_DIVIDER;
  // check for upper- and undervoltage
  if (batteryVoltage > BATTERY_UPPERVOLTAGE_DETECTOR_THRESHOLD) {
    // reset the counter
    batteryUndervoltageDetectorCount = 0;
#ifdef INCLUDE_SERIAL_MONITOR
  Serial.printf("batteryVoltage > BATTERY_UPPERVOLTAGE_DETECTOR_THRESHOLD (%d), reset the undervolt counter\n", BATTERY_UPPERVOLTAGE_DETECTOR_THRESHOLD);
  Serial.flush();
#endif    
  } else {
    if (batteryVoltage < BATTERY_UNDERVOLTAGE_DETECTOR_THRESHOLD) {
      // increase the counter and enable the 'isUndervoltageDeepSleep'
      batteryUndervoltageDetectorCount++;
      ledFlash(5, 125);
#ifdef INCLUDE_SERIAL_MONITOR
  Serial.printf("batteryVoltage (%1.2f) < BATTERY_UNDERVOLTAGE_DETECTOR_THRESHOLD (%d) undervoltageCounter increased to %d\n", batteryVoltage, BATTERY_UPPERVOLTAGE_DETECTOR_THRESHOLD, batteryUndervoltageDetectorCount);
  Serial.flush();
#endif      
      if (batteryUndervoltageDetectorCount > BATTERY_UNDERVOLTAGE_MAXIMUM_COUNTS) {
        isUndervoltageDeepSleep = true;
#ifdef INCLUDE_SERIAL_MONITOR
  Serial.printf("batteryUndervoltageDetectorCount (%d) > BATTERY_UNDERVOLTAGE_MAXIMUM_COUNTS (%d) -> permanently DEEP SLEEP MODE\n", batteryUndervoltageDetectorCount, BATTERY_UPPERVOLTAGE_DETECTOR_THRESHOLD);
  Serial.flush();
#endif        
      }
    }
  }
  // calculate the remaining battery capacity
  for (int n = 0; n < sizeof(scaled_voltage); n++) {
    float step = (max_voltage - min_voltage) / 256;
    if (batteryVoltage > min_voltage + (step * scaled_voltage[n])) {
      batteryCapacity = 100 - n;
      disableBatteryVoltageReading();
      return batteryCapacity;
    }
  }
  disableBatteryVoltageReading();
#endif
  return batteryCapacity;
}