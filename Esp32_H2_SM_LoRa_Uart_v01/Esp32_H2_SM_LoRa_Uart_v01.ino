/*
  This sketch is running on an ESP32-H2 Super Mini device.
  It has two components attached/included:
  1) an UART-interfaced DX-LR02-900T22D LoRa module
  2) the onboard RGB LED

  It shows how to include a receiver and transmitter to your sketch. The RGB LED is
  blinking GREEN when receiving a packet and BLUE when sending some data.

  Important note: you need to make the settings for the LoRa module
  manually BEFORE you start to run this sketch, as the module is, e.g.,
  using a frequency band or transmission power that is not allowed
  in your country.

  For an overview, see https://www.thethingsnetwork.org/docs/lorawan/regional-limitations-of-rf-use/

  I'm strongly recommending my article "Simple LoRa transmissions over several 
  kilometers using UART/Serial LoRa modules", as it shows how to make the settings 
  by an USB interface.
  URL: https://medium.com/@androidcrypto/simple-lora-transmissions-over-several-kilometers-using-uart-serial-lora-modules-6c5abbf464f2

  Second: this sketch is transmitting a small, 4 bytes long packet every
  14 seconds - that could lead to a spamming of the channel in use.
  Please activate the transmission for a short period of time please !
  For transmission uncomment '#define ENABLE_TRANSMISSION true' below

  In Arduino, use ESP32H2 Dev Module for compiling and uploading.
  Don't forget to enable the option "USB CDC on boot enabled" in Tools menue
  to see output on Serial Monitor.
*/

/*
  Version Management
12.02.2026 V01 Initial programming
*/

/*
Settings for the LoRa module for Europe
Commands: +++
          AT+HELP
for Channel: AT+CHANNEL27 
see DX-LR02-900T22D Serial port application guide.pdf for 
more informations how to change the settings.
===================================
LoRa Parameter:
+VERSION=V2.3.1
MODE:0
LEVEL:0 >> 4557.291992bps
SLEEP:2
Frequency:869500000hz >> 27
MAC:ff,ff
Bandwidth:0
Spreading Factor:7
Coding rate:2
CRC:0(false)
Preamble:8
IQ:0(false)
Power:0dBm
=================================  
*/

// ------------------------------------------------------------------
// program information
const char* PROGRAM_VERSION = "ESP32-H2 Super Mini LoRa UART module V01";

//#define ENABLE_TRANSMISSION true
#define TEST_48MHZ_CPU_FREQUENCY

#include "LORA_UART.h"
#include "ONBOARD_LEDS.h"

const long TRANSMISSION_INTERVAL_MILLIS = 14000;  // 14 seconds
long lastTransmissionMillis = 0;

// ------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(PROGRAM_VERSION);

#ifdef TEST_48MHZ_CPU_FREQUENCY
  // test that the sensor is working on 48 MHz CPU frequency as well
  Serial.printf("CPU frequency before setting: %d MHz\n", getCpuFrequencyMhz());
  setCpuFrequencyMhz(48);
  Serial.printf("CPU frequency after  setting: %d MHz\n", getCpuFrequencyMhz());
#else
  Serial.printf("CPU frequency: %d MHz\n", getCpuFrequencyMhz());
#endif  

  setupOnboardLeds();

  // Connect to LoRa device
  setupLoraUart();

  onboardLedFlashWhite(1, 125);  // flash the LED in WHITE
  Serial.println(F("The setup is complete for receiving and transmissions"));
#ifdef ENABLE_TRANSMISSION
  Serial.printf("A transmission is done every %d seconds\n", TRANSMISSION_INTERVAL_MILLIS / 1000);
#endif
  delay(1000);
}

void loop() {
  loraUartLoop();

  if (isReceived) {
    isReceived = false;
    onboardLedFlashGreen(1, 125);  // flash the LED in GREEN
    // here is the place to work with the data, I'm just printing the received packet
    // HEX and UTF-8 encodings and displaying HEX encoding
    printRxBuffer(rxBufferLength);
    printRxBufferChar(rxBufferLength);
  }

#ifdef ENABLE_TRANSMISSION
  // this is the transmission part
  if (millis() - lastTransmissionMillis > TRANSMISSION_INTERVAL_MILLIS) {
    txCounter++;

    // just six bytes
    // prepare the array, we are using the arrayRW method from the LoRa library
    uint8_t txLength = 0;
    beginarrayRW(txBuffer, 0);    // start writing to control array
    arrayWriteUint8(0x41);        // 1 byte static for identifying 'A'
    arrayWriteUint32(txCounter);  // 4 bytes
    arrayWriteUint8(0x44);        // 1 byte static for identifying 'D'
    txLength = endarrayRW() + 1;  // this returns the number of array bytes written, is 6 bytes

    Serial.printf("TX (%d): ", txLength);
    Serial.println();

    if (transmitData(txLength)) {
      printTxBuffer(txLength);
      Serial.flush();
    }
    onboardLedFlashBlue(1, 125);  // flash the LED in WHITE
    lastTransmissionMillis = millis();
  }
#endif
}

/*
ESP32-H2 Super Mini LoRa UART module V01
LORA_SERIAL timeout is 10 ms
The setup is complete for receiving and transmissions
rxBuffer ( 5): 61 62 63 0D 0A 
rxBuffer ( 5): abc
TX (6): 
txBuffer ( 6): 41 01 00 00 00 44 
TX (6): 
txBuffer ( 6): 41 02 00 00 00 44 

USB-Receiver
[2026-02-12 11:51:50.944]# RECV ASCII/6 <<<
A   D
[2026-02-12 11:52:05.196]# RECV HEX/6 <<<
41 02 00 00 00 44
*/