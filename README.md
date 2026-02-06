# ESP32-H2 Super Mini Development Board as the basis for a battery-powered LoRa transmitter project

This repository accompanies a large project centered around the small ESP32-H2 Super Mini development board. It consists of several sub-projects that deal 
with the topics SHT41 sensor, MAX17043 battery measuring device, LoRa module, e-paper display and onboard LEDs. The end result is a LoRa transmitter that 
can transmit data over long distances.

### Please note: this is an ongoing project, and I will update this repository accordingly when new tutorials are available, please be patient in the meantime.

![Image 1](./images/heltec_wl_v2_ccs811_01_600w.png)

These individual tutorials are supported by a comprehensive resource where all programs and libraries are stored together. I have also saved supplementary 
materials such as pinouts and board schematics here. The following information is available (or will be provided soon):

- Overview tutorial "**[ESP32-H2 Super Mini-Small form factor, big impact (LoRa, ePaper, environment sensor, and battery powered and monitored)](https://medium.com/@androidcrypto/esp32-h2-super-mini-small-form-factor-big-impact-lora-epaper-environment-sensor-and-battery-bcd469ee633a)**"
- "**Use the onboard LEDs on an ESP32-H2 Super Mini Development Board**" <soon>
- "**Use an SSD1306 OLED display with an ESP32-H2 Super Mini Development Board**"" <soon>
- "**Use an SHT41 Environment Sensor with an ESP32-H2 Super Mini Development Board and get indoor temperature and humidity**"" <soon>
- "**Use a MAX17043 battery voltage meter with an ESP32-H2 Super Mini Development Board and get the battery voltage of your lithium battery and its remaining capacity**"" <soon>
- "**Use a UART-interfaced LoRa module with an ESP32-H2 Super Mini Development Board and transmit and receive LoRa packets**"" <soon>
- "**Use the Deep Sleep mode to reduce power consumption on an ESP32-H2 Super Mini Development Board**"" <soon>
- "**Run current measurements of the individual modules connected to an ESP32-H2 Super Mini Development Board**"" <soon>
- "**Build an external circuit to de-energize parts of our setup connected to an ESP32-H2 Super Mini Development Board**"" <soon>
- the final project: "**ESP32-H2 Super Mini Board with SHT41 sensor, MAX17403 monitored battery, e-paper display and LoRa data transmission**"" <soon>

The additional material is located in the subfolder "Material":
- the pinout of the ESP32-H2 Super Mini Development board
- the schematic of the ESP32-H2 Super Mini Development board

The code of each sketch is in sepate folders. All libraries used within the sketches should get installed with the Arduino Library Manager, but in case of network or other problems, 
I'm including them in the subfolder "Libraries".

### Sketches for the tutorials

- Find the "Onboard LEDs" sketch in **Esp32_H2_SM_OnboardLeds_v02** folder


## Development Environment (Arduino)
````plaintext
Arduino IDE Version 2.3.6 (Windows)
arduino-esp32 boards Version 3.3.5 (https://github.com/espressif/arduino-esp32) that is based on Espressif ESP32 Version 5.5.1
````

Last update: February, 6 th. 2026
