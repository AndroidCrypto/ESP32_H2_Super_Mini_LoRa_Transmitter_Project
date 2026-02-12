/*
  Management of an external WeAct 1.54-inches e-paper display
  For more information on full and partial refresh, see this tutorial:
  https://www.makerguides.com/partial-refresh-e-paper-display-esp32/

  Please note that a fullRefresh is forced after MAXIMUM_PARTIAL_REFRESHES.

  08.02.2026 V01 Initial version
  
*/

#ifndef IS_WEACT_154_EPAPER
#define IS_WEACT_154_EPAPER
#endif

// Epaper display
#define ENABLE_GxEPD2_GFX 0
#include "GxEPD2_BW.h" // https://github.com/ZinggJM/GxEPD2 version 1.6.6
#include <Fonts/FreeMonoBold9pt7b.h>
#include <U8g2_for_Adafruit_GFX.h>  // https://github.com/olikraus/U8g2_for_Adafruit_GFX version 1.8.0

#define EPD_BUSY 24  // to EPD BUSY 24 is labled as 'TX'
#define EPD_CS   14  // to EPD CS
#define EPD_RST  12  // to EPD RST
#define EPD_DC   10  // to EPD DC
#define EPD_SCK  23  // to EPD CLK 23 is labled as 'RX'
#define EPD_MISO -1  // Master-In Slave-Out not used, as no data from display
#define EPD_MOSI 11  // to EPD DIN / SDA

// 1.54'' EPD Module
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> epd(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEH0154D67 200x200, SSD1681
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;  // Select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
#define DISPLAY_ROTATION_TERMINAL_RIGHT 1 // the 8 wires are going to the right side of the display
#define DISPLAY_TEXT_SIZE_MEDIUM 2
#define DISPLAY_LINE_DISTANCE 28
#define DISPLAY_OFFSET_X 5
#define DISPLAY_OFFSET_Y 5
const long DISPLAY_REFRESH_INTERVAL_MILLIS = 2000;  // 2 seconds

/*
Wiring of the display to the ESP32-H2 Supermini device. Please 
double-check the pins on the PCB and colors of your cable to 
avoid any damage of the display..
Nr name  color   ESP32-H2 SM pin
1  BUSY  violet  24 (labeled as 'TX')
2  RES   orange  12
3  D/C   white   13
4  CS    blue    14
5  SCL   green   23 (labeled as 'RX')
6  SDA   yellow  11 ('SDA' terminal is known as 'MOSI')
7  GND   black   GND
8  VCC   red     VCC is 3.3 volt, not 5 volt !

On the board use '4-Lines SPI' (upper solder pads closed)
*/

const char* PROJECT_NAME = "ESP32-H2 Sensor";
const char* DISPLAY_SHT41 = "SHT41";
const char* DISPLAY_TEMP = "Temp:";
const char* DISPLAY_HUMID = "Humid:";
const char* DISPLAY_MAX1704 = "MAX17043";
const char* DISPLAY_VOLT = "Volt:";
const char* DISPLAY_CAP = "Cap:";

RTC_DATA_ATTR bool initial = true; // required for deep sleep to avoid flickering
RTC_DATA_ATTR uint16_t partialRefreshCounter = 0; 
/*
As the author, Stefan Maetschke, assumes, is a full refresh each day a good recommendation.
As for the tutorial, I'm running the deep sleep for about 30 seconds. If a day has
24 hours * 60 minutes * 60 seconds = 86400 seconds, a full refresh should be done
86400 / 30 = 2880 partial refreshes:
*/
const uint16_t MAXIMUM_PARTIAL_REFRESHES = 2880; // depending on deep sleep duration, see above
//const uint16_t MAXIMUM_PARTIAL_REFRESHES = 4; // testing the full refresh after 4 partial refreshes

void setupEpaperDisplay() {
  SPI.end();
  SPI.begin(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS);
  epd.init(115200, initial, 50, false);
  epd.setRotation(DISPLAY_ROTATION_TERMINAL_RIGHT);
  epd.setTextSize(DISPLAY_TEXT_SIZE_MEDIUM);
  epd.setTextColor(GxEPD_BLACK);
  Serial.println(F("The Epaper display is set up"));
  Serial.flush();
}

void drawFull(const void* pv) {
  uint8_t lineCounter = 0;
  epd.setFullWindow();
  // project name
  epd.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(PROJECT_NAME);
  lineCounter++;
  // sensor name
  epd.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(DISPLAY_SHT41);
  lineCounter++;
  // temperature
  epd.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(DISPLAY_TEMP);
  lineCounter++;
  // humidity
  epd.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(DISPLAY_HUMID);
  lineCounter++;
  // sensor name
  epd.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(DISPLAY_MAX1704);
  lineCounter++;
  // battery voltage
  epd.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(DISPLAY_VOLT);
  lineCounter++;
  // battery capacity
  epd.setCursor(DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(DISPLAY_CAP);
  initial = false;
}

void checkFullRefresh() {
  if (partialRefreshCounter >= MAXIMUM_PARTIAL_REFRESHES) {
    epd.drawPaged(drawFull, 0);
    partialRefreshCounter = 1; // reset the counter
  }
}  

void drawPartial(const void* pv) {
  partialRefreshCounter++;
  uint8_t lineCounter = 0;
  uint16_t x = 102, y = (1 * DISPLAY_LINE_DISTANCE), w = 92, h = (6 * DISPLAY_LINE_DISTANCE);
  epd.setPartialWindow(x, y, w, h);
  // Battery Undervolt Detection Count
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(budc);
  lineCounter++;
  // temperature
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(String(sht41_temperature, 1) + " C ");
  lineCounter++;
  // humidity
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(String(sht41_humidity, 1) + " % ");
  lineCounter++;
  // sensor name
  // here we are display the increasing readings counter
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print("C:" + String(txCounter) + "   ");
  lineCounter++;
  // battery voltage
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(String(batteryVoltage, 2) + " V ");
  lineCounter++;
  // battery capacity
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(String(batteryCapacity) + " % ");
  epd.drawRect(x, y, w, h, GxEPD_BLACK);
}

void drawPartialOld(const void* pv) {
  uint8_t lineCounter = 0;
  uint16_t x = 102, y = (2 * DISPLAY_LINE_DISTANCE), w = 92, h = (5 * DISPLAY_LINE_DISTANCE);
  epd.setPartialWindow(x, y, w, h);
  // temperature
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  //epd.print(millis());
  epd.print(String(sht41_temperature, 1) + " C ");
  lineCounter++;
  // humidity
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(String(sht41_humidity, 1) + " % ");
  lineCounter++;
  // sensor name
  // here we are display the increasing readings counter
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print("C:" + String(txCounter) + "   ");
  lineCounter++;
  // battery voltage
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(String(batteryVoltage, 2) + " V ");
  lineCounter++;
  // battery capacity
  epd.setCursor(x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y + (lineCounter * DISPLAY_LINE_DISTANCE));
  epd.print(String(batteryCapacity) + " % ");
  epd.drawRect(x, y, w, h, GxEPD_BLACK);
}

/*
I asked the author, Stefan Maetschke, for a partial-full refresh cycle,
here is his answer:
Hi,
I assume it will depend on the display and what is displayed.
I definitely see ghosting when using partial updates and the danger is
that these artefacts remain permanently.
I therefore typically do a full refresh every hour or a least once a day.
I have clock running with a partial refresh every minute and a full refresh
every hour and the display is still fine after a year.
So, a full refresh every 5 partial ones is probably not necessary.
*/
