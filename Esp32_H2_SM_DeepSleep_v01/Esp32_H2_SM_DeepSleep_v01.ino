/*
  This sketch is explained in my tutorial 
  "Using Deep Sleep mode to reduce power consumption (automatic waking up after 
  a certain sleep time using a timer)"

  It shows how to setup the timer-based Deep Sleep mode on an ESP32-H2 microcontroller
  and some features.

  In Arduino, use ESP32H2 Dev Module for compiling and uploading.
  Don't forget to enable the option "USB CDC on boot enabled" in Tools menue
  to see output on Serial Monitor.

  A fatal error occurred: Could not open COM28, the port is busy or doesn't exist.
  (could not open port 'COM28': FileNotFoundError(2, 'Das System kann die angegebene Datei nicht finden.', None, 2))
  Hint: Check if the port is correct and ESP connected
  esptool v5.1.0
  Serial port COM28:
  Failed uploading: uploading error: exit status 2

  This is a typical error when you try to upload a sketch during deep sleep. Simply press the
  RESET button a short time before the upload is starting.
*/

/*
  Version Management
18.02.2026 V01 Initial programming  
*/

// ------------------------------------------------------------------
// Place all variables or code below this line
// ------------------------------------------------------------------

char* PROGRAM_VERSION = "ESP32-H2 Deep Sleep mode V01";

#include "ONBOARD_LEDS.h"
#include "DEEP_SLEEP_MANAGEMENT.h"

uint32_t regularCounter = 0;
RTC_DATA_ATTR uint32_t rtcCounter = 0;
RTC_DATA_ATTR bool toggleCpuFrequency = false;
#define DELAY_BEFORE_DEEP_SLEEP_MILLIS 10000  // 10 seconds
#define LED_LIGHTING_DURATION_MILLIS 2000     // 2 seconds

void setAllGpiosToInput() {
  // onboard leds
  pinMode(ONBOARD_LED_PIN, INPUT);      // this is a small blue LED
  pinMode(ONBOARD_RGB_LED_PIN, INPUT);  // this is a WS2812 RGB LED
  // ... add more pins you are using in your sketch, e.g. I2C, SPI or UART interfaces
}

void setup() {
  // 01) get the wakeup reason and print it on the Serial Monitor
  // 0=first start/reset, 2=ext0 button, 3=ext1 button, 4=timer
  // should get called first
  getWakeupReason();

  Serial.begin(115200);
  delay(500);
  Serial.println(PROGRAM_VERSION);

  setupOnboardLeds();

  if (esp32WakeupReason == 0) {
    // this is a restart
    onboardLedFlashWhite(1, 250);
    setRgbLedOff();
  } else {
  onboardLedFlash(1, 250);
  }
  delay(LED_LIGHTING_DURATION_MILLIS);


  Serial.printf("regularCounter: %d rtcCounter %d : ", regularCounter, rtcCounter);
  Serial.printf("CPU frequency before setting: %d MHz\n", getCpuFrequencyMhz());
  // toggle the cpu frequency between 48 and 96 mhz
  toggleCpuFrequency = !toggleCpuFrequency;
  if (toggleCpuFrequency) {
    setCpuFrequencyMhz(48);
    onboardLedFlashGreen(3, 250);
  } else {
    setCpuFrequencyMhz(96);
    onboardLedFlashRed(3, 250);
  }
  Serial.printf("CPU frequency after  setting: %d MHz\n", getCpuFrequencyMhz());
  delay(LED_LIGHTING_DURATION_MILLIS);

  // increase both counters to see the difference
  regularCounter++;
  rtcCounter++;

  printWakeupReason();

  // run your tasks
  // color sequence depends on the cpu frequency
  if (getCpuFrequencyMhz() == 96) {
    setRgbLedRed(10);
    delay(LED_LIGHTING_DURATION_MILLIS);
    setRgbLedOff();
    setRgbLedGreen(10);
    delay(LED_LIGHTING_DURATION_MILLIS);
    setRgbLedOff();
    setRgbLedBlue(10);
    delay(LED_LIGHTING_DURATION_MILLIS);
    setRgbLedOff();
  } else {
    // 48 MHz
    setRgbLedYellow(10);
    delay(LED_LIGHTING_DURATION_MILLIS);
    setRgbLedOff();
    setRgbLedMint(10);
    delay(LED_LIGHTING_DURATION_MILLIS);
    setRgbLedOff();
    setRgbLedPurple(10);
    delay(LED_LIGHTING_DURATION_MILLIS);
    setRgbLedOff();
  }
  // ...
  // your tasks are done

  // the deep sleep is starting now
  /*
  I'm adding a pause to give you some time to access your ESP32-H2 board,
  e.g. for uploading new code. Otherwise, the running period is very short 
  and you have to press these buttons on the board to run e.g. an upload:
  press and hold BOOT button, press RESET button once, release BOOT button
  now the processor is in BOOT/DOWNLOAD mode and waiting for an upload.
*/
  delay(DELAY_BEFORE_DEEP_SLEEP_MILLIS);

  // blink the onboard LED twice, followed by a Deep Sleep
  onboardLedFlash(2, 125);
  setAllGpiosToInput();
  setupDeepSleep();
  runDeepSleep();

  // nothing beyond this point is executed
  Serial.println(F("IS YOU SEE THIS PRINTOUT THE DEEP SLEEP IS NOT RUN !"));
}

void loop() {
  // nothing to do here
}
