/*
  This file takes all the variables and code for using the deep sleep mode on an
  ESP32 device.
*/

// This is activated by #define USE_DEEP_SLEEP_MODE

#ifndef IS_DEEP_SLEEP_MANAGEMENT
#define IS_DEEP_SLEEP_MANAGEMENT
#endif

const uint16_t DEEP_SLEEP_TIME_SECONDS = 33;  // 33 seconds
#define uS_TO_S_FACTOR 1000000                // Conversion factor for micro seconds to seconds

uint8_t esp32WakeupReason = 0;  // default after uploading and manual reset

void getWakeupReason() {
  esp32WakeupReason = (uint8_t)esp_sleep_get_wakeup_cause();
}

void printWakeupReason() {
  Serial.printf("The wakeup reason number was %d ", esp32WakeupReason);
  if (esp32WakeupReason == 0) {
    Serial.println("(reset)");
  } else if (esp32WakeupReason == 2) {
    Serial.printf("(ext0 on a GPIO\n");
  } else if (esp32WakeupReason == 4) {
    Serial.println("(timer)");
  } else {
    Serial.println("(unknown)");
  }
  Serial.flush();
}

void checkForPermanentDeepSleep() {
#ifdef IS_BATTERY_MANAGEMENT
  // check for "isUndervoltageDeepSleep == true" and remain in deep sleep
  // but when wakeup by reset reset the counter
  if (esp32WakeupReason != 0) {
    if (isUndervoltageDeepSleep == true) {
      // no timer or other wakeup
      esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
      // go into the sleep
      esp_deep_sleep_start();
    }
  } else {
    if (isUndervoltageDeepSleep == true) {
      // wakeup by reset button, reset all data
      isUndervoltageDeepSleep = false;
      batteryUndervoltageDetectorCount = 0;
    }
  }
#endif
}

void setupDeepSleep() {
  // prepare the processor for deep sleep
  // see https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/sleep_modes.html
  // this is for the permanent deep sleep mode when undervoltage was detected
  if (isUndervoltageDeepSleep) {
    // no timer or other wakeup
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
  } else {
    // Configure wake source
    esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME_SECONDS * uS_TO_S_FACTOR);  // Timer wake up
  }
}

void runDeepSleep() {
  esp_deep_sleep_start();
}