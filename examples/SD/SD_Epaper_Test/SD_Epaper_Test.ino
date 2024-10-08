
/**
 * @file      SD_Epaper_Test.ino
 * @author    Liang
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-04-01
 *
 */

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include "utilities.h"
#include "Adafruit_GFX.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

#if defined(LILYGO_S3_E_PAPER_V_1_0)
#include <GxDEPG0213BN/GxDEPG0213BN.h> // 2.13" b/w  form DKE GROUP
#include<GxDEPG0213BN/BitmapExamples.h>
#endif

SPIClass SDSPI(HSPI);

GxIO_Class io(SDSPI, EDP_CS_PIN, EDP_DC_PIN, EDP_RSET_PIN);
GxEPD_Class display(io, EDP_RSET_PIN, EDP_BUSY_PIN);


void setup()
{

    Serial.begin(115200);

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
    Serial.println("EDP_MODE");

    if(!SD.begin(SDCARD_CS,SDSPI)){
        Serial.println("SD init failed");
        display.setRotation(3);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 55);
        display.println("SD init failed");
        display.update();
        return;
    }else {
        Serial.println("SD init success");
        Serial.printf("SD Card insert:%.2f GB",SD.cardSize() / 1024.0 / 1024.0 / 1024.0);
        display.setCursor(0, 55);
        display.setRotation(3);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.printf("SDCard insert:%.2f GB",SD.cardSize() / 1024.0 / 1024.0 / 1024.0);
        display.update();
    }

}
void loop()
{

}