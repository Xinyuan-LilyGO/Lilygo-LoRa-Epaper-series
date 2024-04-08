#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "utilities.h"

#ifdef HAS_SDCARD
#include <SD.h>
#include <FS.h>
#endif
SPIClass SDSPI(HSPI);

#include "Adafruit_GFX.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxDEPG0213BN/GxDEPG0213BN.h> // 2.13" b/w  form DKE GROUP

#include GxEPD_BitmapExamples
GxIO_Class io(SDSPI, EDP_CS_PIN, EDP_DC_PIN, EDP_RSET_PIN);
GxEPD_Class display(io, EDP_RSET_PIN, EDP_BUSY_PIN);



void initBoard()
{
    Serial.begin(115200);
    Serial.println("initBoard");
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
    // Wire.begin(I2C_SDA, I2C_SCL);

    pinMode(SDCARD_MISO, INPUT_PULLUP);
    SDSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);
    
    display.init(); 
    display.setTextColor(GxEPD_BLACK);
    delay(10);
    display.setRotation(2);
    delay(10);
    display.fillScreen(GxEPD_WHITE);
    delay(10);
    display.drawExampleBitmap(LILYGO_logo, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
    display.update();

    uint16_t x = GxEPD_WIDTH / 2  ;
    uint16_t y = GxEPD_HEIGHT / 2;

    display.fillScreen(GxEPD_WHITE);

    display.setCursor(x, y);                          // start writing at this position
    display.print("SleepTest Running");

#ifdef HAS_SDCARD

    if (!SD.begin(SDCARD_CS, SDSPI)) {
        Serial.println("setupSDCard FAIL");
    } else {
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        Serial.print("setupSDCard PASS . SIZE = ");
        Serial.print(cardSize);
        Serial.println(" MB");
    }
#endif

#ifdef BOARD_LED
    /*
    * T-BeamV1.0, V1.1 LED defaults to low level as trun on,
    * so it needs to be forced to pull up
    * * * * */
#if LED_ON == LOW
    gpio_hold_dis(GPIO_NUM_4);
#endif
    pinMode(BOARD_LED, OUTPUT);
    digitalWrite(BOARD_LED, LED_ON);
#endif


}


