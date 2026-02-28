
/*
  This sketch is running on an ESP32-H2 Super Mini device.
  It has five components attached/included:
  1) a WeAct 1.54-inch 200x200 px Epaper display using the SPI interface
  2) an SHT41 environment sensor for temperature and humidity, interfaced by the I2C interface
  3) a MAX17043 Fuel Gauge Sensor for measuring the battery voltage, interfaced by the same I2C bus as the SHT41 sensor
  4) an UART-interface DX-LR02-900T22D LoRa module
  5) the onboard (blue) status LED

  The measured sensor data are displayed and every 14 seconds a new reading is done.
  The new values are printed by a 'Partial Refresh' of the display to avoid any
  flickering.

  The ESP32-H2 Super Mini development board is not mounted on an Expansion Board, because
  the power consumption is higher with the board. The battery is connected by the
  solder pas on the back side of the board. Unfortunately, the board has no
  battery voltage reading, so I need the MAX17043 sensor for this task. If you don't own
  a MAX17043 sensor, you can alternately use a resistor based voltage divider and read
  the battery voltage with an ADC GPIO pin. Please keep in mind, that the voltage divider
  is drawing even a small current permanentely, so it is very important to use a
  battery with undervoltage protection, or you risk a damaged battery when the battery
  is still connected when the device is off.

  The sensor data from the environment sensor and the battery data are published using
  the LoRa module.

*/

/*
  Version Management
24.02.2026 V05 final code for tutorial  
23.02.2026 V04 Code cleaning
23.02.2026 V03 Included SHT41 library
23.02.2026 V02 This is the Esp32_H2_LoRa_Transmitter_v04 sketch
Final Version
*/

// ------------------------------------------------------------------
// program information
const char* PROGRAM_VERSION = "ESP32-H2 Super Mini LoRa transmitter Final V05";

// ------------------------------------------------------------------
// LoRa module
#include "LORA_UART.h"

#define ENABLE_TRANSMISSION true

// ------------------------------------------------------------------
// Power Switch Module for LoRa module

//#include "driver/rtc_io.h" // required for RTC storage of GPIO pin state during deep sleep
#include "driver/gpio.h"
#define LORA_POWER_SWITCH_PIN 1  // don't forget - HIGH is ON, LOW is OFF
#define LORA_POWER_SWITCH_GPIO_PIN GPIO_NUM_1

// ------------------------------------------------------------------
// SHT41 environment sensor
#include "SHT41_SENSOR.h"

// ------------------------------------------------------------------
// MAX1704 sensor
#include "MAX17043_METER.h"

// ------------------------------------------------------------------
// Epaper display

#include "WEACT_154_EPAPER.h"

// ------------------------------------------------------------------
// Deep Sleep

#include "DEEP_SLEEP_MANAGEMENT.h"

const long TRANSMISSION_INTERVAL_MILLIS = 14000;  // 14 seconds
long lastTransmissionMillis = 0;

// ------------------------------------------------------------------
// onboard LEDs
#include "ONBOARD_LEDS.h"

// ------------------------------------------------------------------

void setup() {

  // get the wakeup reason, 0=first start/reset, 2=ext0 button 4=timer
  getWakeupReason();

  checkForPermanentDeepSleep();

  Serial.begin(115200);
  delay(500);
  Serial.println(PROGRAM_VERSION);

  printWakeupReason();
  if (esp32WakeupReason == 0) {
    initial = true; // force a full refresh of the display
  }

  // setup the power switch module
  pinMode(LORA_POWER_SWITCH_PIN, OUTPUT);
  digitalWrite(LORA_POWER_SWITCH_PIN, HIGH);  // on

  setupOnboardLeds();

  // stop all other radios
  //esp_wifi_stop();
  //esp_bt_controller_disable();

  setupEpaperDisplay();

  if (initial) epd.drawPaged(drawFull, 0);
  epd.hibernate();

  setupSht41();
  setupMax17043();

  // Connect to LoRa device
  setupLoraUart();

  Serial.println(F("The setup is complete for reading and displaying"));
}

void deactivateAllGpioPins() {
  // Onboard LEDs
  pinMode(ONBOARD_LED_PIN, INPUT);
  pinMode(ONBOARD_RGB_LED_PIN, INPUT);
  // I2C
  pinMode(SHT4X_I2C_SDA_PIN, INPUT);
  pinMode(SHT4X_I2C_SCL_PIN, INPUT);
  // SPI for EPD
  pinMode(EPD_BUSY, INPUT);
  pinMode(EPD_CS, INPUT);
  pinMode(EPD_RST, INPUT);
  pinMode(EPD_DC, INPUT);
  pinMode(EPD_SCK, INPUT);
  //pinMode(EPD_MISO, INPUT);
  pinMode(EPD_MOSI, INPUT);
  // UART for LoRa
  pinMode(RXD2_PIN, INPUT);
  pinMode(TXD2_PIN, INPUT);
  /*
  // deactivate in case you are using an onboard OLED
  //pinMode(SDA_OLED, INPUT);
  //pinMode(SCL_OLED, INPUT);
  //pinMode(RST_OLED, INPUT);
*/
}

void loop() {
  loraUartLoop();

  if (isReceived) {
    isReceived = false;
    // here is the place to work with the data, I'm just printing the received packet
    // HEX and UTF-8 encodings and displaying HEX encoding
    printRxBuffer(rxBufferLength);
    printRxBufferChar(rxBufferLength);
  }

  getSht41Values();
  printSht41Values();

  // wake up the meter
  if (isMax17043Sleeping()) {
    setMax17043Wake();
  }
  getMax17043Values();
  printMax17043Values();

  // make a transmission
  txCounter++;
  // prepare the array, we are using the arrayRW method from the LoRa library
  uint8_t dataArraySize = 0;

  beginarrayRW(txBuffer, 0);                          // start writing to control array
  arrayWriteUint8(esp32WakeupReason);                 // 1 byte
  arrayWriteUint32(txCounter);                        // 4 bytes
  arrayWriteUint8(batteryUndervoltageDetectorCount);  // 1 byte
  // sensor data
  arrayWriteFloat(sht41_temperature);  // 4 bytes
  arrayWriteFloat(sht41_humidity);     // 4 bytes
  arrayWriteFloat(sht41_pressure);     // 4 bytes
  // battery data
  arrayWriteFloat(batteryVoltage);   // 4 bytes
  arrayWriteInt16(batteryCapacity);  // 2 bytes
  arrayWriteUint8(254);              // 1 byte static for identifying
  dataArraySize = endarrayRW() + 1;  // this returns the number of array bytes written, is 25 bytes

  uint8_t txLength = dataArraySize;

  Serial.printf("TX (%d): ", dataArraySize);
  Serial.println();
  Serial.flush();

  if (transmitData(txLength)) {
    printTxBuffer(txLength);
  }

  //digitalWrite(LORA_POWER_SWITCH_PIN, HIGH);

  sprintf(budc, " UDC:%1d", batteryUndervoltageDetectorCount);
  Serial.printf("Counter: %d Temperature: %2.1f C Humidity: %2.1f %%rH Battery voltage: %1.2f v Capacity: %d %% UndervoltCounter: %d\n", txCounter, sht41_temperature, sht41_humidity, batteryVoltage, batteryCapacity, batteryUndervoltageDetectorCount);

  checkFullRefresh();
  epd.drawPaged(drawPartial, 0);
  //epd.hibernate();
  epd.powerOff();  // might be the best solution for long lasting deep sleep

  onboardLedFlash(1, 125);  // a single flash before we are going into deep sleep
  delay(5000); // this is just to lengthen the "online" time to give you time to bring
  // the device in uploading mode during development
  onboardLedFlash(2, 125);

  //delay(DISPLAY_REFRESH_INTERVAL_MILLIS);

  // now it is time to go into deep sleep
  setupDeepSleep();

  // The SHT40/41 is going into a sleep mode after a measurement, so we don't need
  // for a dedicated command to do so
  // bring the MAX1704 into sleep mode
  //_fuelGauge.sleep();
  setMax17043Sleep();

  Serial.println(F("Going into Deep Sleep"));

  // change the gpio pins to input state
  digitalWrite(LORA_POWER_SWITCH_PIN, LOW);  // put the power switching circuit 'off'
  deactivateAllGpioPins();

  // Configure timer to wake up in 10 seconds
  setupDeepSleep();
  pinMode(LORA_POWER_SWITCH_PIN, INPUT);
  // go into the sleep
  runDeepSleep();
  delay(20000); // this is never called !
}
