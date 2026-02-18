/*
  This file takes all the variables and code for using the deep sleep mode on an
  ESP32 device.

  The ESP32-H2 API reference point is:
  https://docs.espressif.com/projects/esp-idf/en/stable/esp32h2/api-reference/system/sleep_modes.html
  This API header file gives you some explanations on the features like available GPIOs:
  https://github.com/espressif/esp-idf/blob/v5.5.2/components/esp_hw_support/include/esp_sleep.h
*/

// This is activated by #define USE_DEEP_SLEEP_MODE

#ifndef IS_DEEP_SLEEP_MANAGEMENT
#define IS_DEEP_SLEEP_MANAGEMENT
#endif

const uint16_t DEEP_SLEEP_TIME_SECONDS = 10;  // 10 seconds
#define uS_TO_S_FACTOR 1000000                // Conversion factor for micro seconds to seconds

uint8_t esp32WakeupReason = 0;  // default after uploading and manual reset

RTC_DATA_ATTR uint8_t deepSleepCounter = 0;

void getWakeupReason() {
  esp32WakeupReason = (uint8_t)esp_sleep_get_wakeup_cause();
}

void printWakeupReason() {
  Serial.printf("The wakeup reason number was %d ", esp32WakeupReason);
  if (esp32WakeupReason == 0) { //  ESP_SLEEP_WAKEUP_UNDEFINED
    Serial.println("(reset)");
  } else if (esp32WakeupReason == 2) { // ESP_SLEEP_WAKEUP_EXT0
    Serial.printf("(ext0 on a GPIO\n");
  } else if (esp32WakeupReason == 3) { // ESP_SLEEP_WAKEUP_EXT1
    Serial.printf("(ext1 on a GPIO\n");  
  } else if (esp32WakeupReason == 4) { // ESP_SLEEP_WAKEUP_TIMER
    Serial.println("(timer)");
  } else {
    Serial.println("(unknown)");
  }
  Serial.flush();
}

void setupDeepSleep() {
  // prepare the processor for deep sleep
  // see https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/sleep_modes.html
  // Configure wake source
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME_SECONDS * uS_TO_S_FACTOR);  // Timer wake up
}

void runDeepSleep() {
  esp_deep_sleep_start();
}