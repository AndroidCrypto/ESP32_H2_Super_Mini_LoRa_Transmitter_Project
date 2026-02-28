/*
  This file takes all the variables and code for using the UART-interfaced
  LoRa module DX-LR02-900T22D.

*/

// This is activated by #define LORA_UART

#ifndef IS_LORA_UART
#define IS_LORA_UART
#endif

#include "arrayRW.h" // necessary for array building

// ------------------------------------------------------------------
// LoRa module
#define LORA_SERIAL Serial1  // using the second UART interface
#define RXD2_PIN 3 // Note: the TXD pin of the LoRa module is connected to the RXD2 pin of the ESP32-H2 SM
#define TXD2_PIN 2 // Note: the RXD pin of the LoRa module is connected to the TXD2 pin of the ESP32-H2 SM

/*
Wiring LoRa module - ESP32-H2 SM
M0  - not connected
M1  - not connected
RXD - GPIO 2         yes, the RXD pin of the LoRa module is connected to the TX pin of the ESP32-H2 SM
TXD - GPIO 3         yes, the TXD pin of the LoRa module is connected to the RX pin of the ESP32-H2 SM
AUX - not connected
VCC - 3.3 pin
GND - GND pin
*/
const uint16_t RX_TIMEOUT_MILLIS = 10;

const uint8_t RX_BUFFER_SIZE = 255;
uint8_t rxBuffer[RX_BUFFER_SIZE];
int rxBufferLength = 0;
bool isReceived = false;

const uint8_t TX_BUFFER_SIZE = 255;
uint8_t txBuffer[TX_BUFFER_SIZE];
int txBufferLength = 0;
RTC_DATA_ATTR uint32_t txCounter = 0; // save in RTC memory for deep sleep

// predeclarations
void printRxBuffer(uint8_t length);
void printRxBufferChar(uint8_t length);
void printTxBuffer(uint8_t length);

void setupLoraUart() {
  // Connect to LoRa device
  LORA_SERIAL.begin(9600, SERIAL_8N1, RXD2_PIN, TXD2_PIN);
  // set a timeout for receiving data
  // default is LORA_SERIAL timeout is 1000 ms
  const uint16_t RX_TIMEOUT_MILLIS = 10;
  LORA_SERIAL.setTimeout(RX_TIMEOUT_MILLIS);
  Serial.printf("LORA_SERIAL timeout is %d ms\n", LORA_SERIAL.getTimeout());
}

void loraUartLoop() {
  // even if we don't need this - this clears the buffer of the LoRa module
  // this is the receiver part
  // read messages from the device
  while (LORA_SERIAL.available() > 0) {
    rxBufferLength = LORA_SERIAL.readBytes(rxBuffer, sizeof(rxBuffer));
    if (rxBufferLength > 0) {
      isReceived = true;
    }
  }
}

// this is transmitting the data in txBuffer with dataLength
bool transmitData(uint8_t dataLength) {
  txBufferLength = dataLength;
  LORA_SERIAL.write(txBuffer, txBufferLength);
  return true;
}

// LoRa helper methods

void printRxBuffer(uint8_t length) {
  Serial.printf("rxBuffer (%2d): ", length);
  for (uint8_t i = 0; i < length; i++) {
    Serial.printf("%02X ", rxBuffer[i]);
  }
  Serial.println();
}

void printRxBufferChar(uint8_t length) {
  Serial.printf("rxBuffer (%2d): ", length);
  for (uint8_t i = 0; i < length; i++) {
    Serial.print((char)rxBuffer[i]);
  }
  Serial.println();
}

void printTxBuffer(uint8_t length) {
  Serial.printf("txBuffer (%2d): ", length);
  for (uint8_t i = 0; i < length; i++) {
    Serial.printf("%02X ", txBuffer[i]);
  }
  Serial.println();
}
