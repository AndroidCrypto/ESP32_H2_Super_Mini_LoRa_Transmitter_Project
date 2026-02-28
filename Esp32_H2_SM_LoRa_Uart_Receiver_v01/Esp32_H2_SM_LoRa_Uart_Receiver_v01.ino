/*
  This sketch is running on an ESP32-H2 Super Mini device.
  It has three components attached/included:
  1) an UART-interfaced DX-LR02-900T22D LoRa module
  2) the onboard RGB LED
  3) an external SSD1306 OLED display

  It shows how to include a receiver and transmitter to your sketch. The RGB LED is
  blinking GREEN when receiving a packet and BLUE when sending some data.
  A received and transmitted packet are shown on the display in HEX encoding.

  This is a receiver sketch to the Esp32_H2_SM_LoRa_Transmitter_v05 sketch,
  as it decodes the received packet and displays the data on the OLED display
  and print the data on the Serial Monitor.

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
27.02.2026 V01 Initial programming
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
const char* PROGRAM_VERSION = "ESP32-H2 Super Mini LoRa UART Receiver V01";

//#define ENABLE_TRANSMISSION true
#define TEST_48MHZ_CPU_FREQUENCY

#include "LORA_UART.h"
#include "ONBOARD_LEDS.h"
#include "SSD1306_OLED.h"

const long TRANSMISSION_INTERVAL_MILLIS = 14000;  // 14 seconds
long lastTransmissionMillis = 0;

char buf[4];                        // a buffer to print hex values
uint32_t rxCounter = 0;             // accepted packets
uint32_t rxNACounter = 0;           // not accepted packets

// ------------------------------------------------------------------

void displayPacketData(uint8_t wur, uint32_t rxCnt, uint32_t txCnt, uint32_t othCnt, uint8_t undervoltBattery, float bVolt, float bCap, float temp, float humid) {
  oDisplay.clear();
  oDisplay.setTextAlignment(TEXT_ALIGN_LEFT);
  oDisplay.setFont(Monospaced_plain_18);
  if (undervoltBattery < 3) {
    oDisplay1 = String(bVolt) + "V " + String(bCap, 0) + "% " + String(undervoltBattery);
  } else {
    oDisplay1 = String(bVolt) + "V " + String(bCap, 0) + "% " + String(undervoltBattery) + "!!";
  }
  oDisplay.drawString(0, 0, oDisplay1);
  oDisplay2 = String(temp, 1) + "C H:" + String(humid, 0) + "%";
  oDisplay.drawString(0, 17, oDisplay2);
  oDisplay.setFont(Monospaced_plain_13);
  if (wur == 0) {
    oDisplay3 = "R T:" + String(txCnt) + " R:" + String(rxCnt);
  } else if (wur == 2) {
    oDisplay3 = "E T:" + String(txCnt) + " R:" + String(rxCnt);
  } else if (wur == 4) {
    oDisplay3 = "T T:" + String(txCnt) + " R:" + String(rxCnt);
  } else {
    oDisplay3 = "U T:" + String(txCnt) + " R:" + String(rxCnt);
  }
  oDisplay4 = "Other Rec:" + String(othCnt);
  oDisplay.drawString(0, 33, oDisplay3);
  oDisplay.drawString(0, 46, oDisplay4);
  oDisplay.display();
}


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

  setupSsd1306();
  oDisplay1 = "ESP32-H2 Super Mini";
  oDisplay2 = "OLED Display SSD1306 I2C";
  oDisplay3 = "DX-SMART DX-LR02-900T22D";
  oDisplay4 = "LoRa module 863-915 MHz";
  oDisplay5 = "with UART interface";
  oDisplayData();

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

// this is the decoding part of the 25 bytes long packet
beginarrayRW(rxBuffer, 0);                                    // start reading from array at location 0
uint8_t esp32WakeupReason = arrayReadUint8();                 // 1 byte
uint32_t txPacketCounter = arrayReadUint32();                 // 4 bytes
uint8_t batteryUndervoltageDetectorCount = arrayReadUint8();  // 1 byte
// sensor data
float temperature = arrayReadFloat();  // 4 bytes
float humidity = arrayReadFloat();     // 4 bytes
float pressure = arrayReadFloat();     // 4 bytes
// battery data
float batteryVoltage = arrayReadFloat();     // 4 bytes
int16_t batteryCapacity = arrayReadInt16();  // 2 bytes
uint8_t footer = arrayReadUint8();           // 1 byte, should be static 0xFE
uint8_t packetLength = endarrayRW() + 1;     // this returns the number of array bytes read (25)

// simple verification that we should display the data
if (footer == 0xFE) {
  // allowed packet
  rxCounter++;
} else {
  rxNACounter++;
  return;
}

// print the decoded data on the Serial Monitor
Serial.printf("WUR: %d TXCnt: %d UndervoltCnt: %d batVolt %f batCap %d Temp: %f C Humid %f %% Press %3.0f HPa\n", esp32WakeupReason, txPacketCounter, batteryUndervoltageDetectorCount, batteryVoltage, batteryCapacity, temperature, humidity, pressure);

// display the received data
displayPacketData(esp32WakeupReason, rxCounter, txPacketCounter, rxNACounter, batteryUndervoltageDetectorCount, batteryVoltage, batteryCapacity, temperature, humidity);

/*
    oDisplay.resetDisplay();
    oDisplay.println("<-- Received " + String(rxBufferLength) + " bytes");
    for (uint8_t i = 0; i < rxBufferLength; i++) {
      sprintf(buf, "%02X ", rxBuffer[i]);
      oDisplay.print(buf);
      if ((i + 1) % 8 == 0) oDisplay.println();
    }
    oDisplay.println();
    */
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
      oDisplay.resetDisplay();
      oDisplay.println("--> Transmitted " + String(txLength) + " bytes");
      for (uint8_t i = 0; i < txLength; i++) {
        sprintf(buf, "%02X ", txBuffer[i]);
        oDisplay.print(buf);
        if ((i + 1) % 8 == 0) oDisplay.println();
      }
      oDisplay.println();
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