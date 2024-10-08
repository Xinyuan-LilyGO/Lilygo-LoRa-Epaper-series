/**
 * @file      GxEPD_picture_examples.ino
 * @author    Liang
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-04-01
 *
 */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "utilities.h"
#include "image.h"
#include "Adafruit_GFX.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

#if defined LILYGO_S3_E_PAPER_V_1_0
#include <GxDEPG0213BN/GxDEPG0213BN.h> // 2.13" b/w  form DKE GROUP
#elif
#endif

#include GxEPD_BitmapExamples
GxIO_Class io(SPI, EDP_CS_PIN, EDP_DC_PIN, EDP_RSET_PIN);
GxEPD_Class display(io, EDP_RSET_PIN, EDP_BUSY_PIN);

void setup()
{
    Serial.begin(115200);

    pinMode(EDP_MISO_PIN, INPUT_PULLUP);
    SPI.begin(EDP_CLK_PIN, EDP_MISO_PIN, EDP_MOSI_PIN,EDP_CS_PIN);

    display.init();
    display.setTextColor(GxEPD_BLACK);
    delay(10);
    display.setRotation(2);
    delay(10);
    display.fillScreen(GxEPD_WHITE);
    delay(10);
    display.drawExampleBitmap(gImage_pkq0, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
    display.update(); 
    delay(1000);
    display.drawExampleBitmap(LILYGO_logo, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
    display.update();
    Serial.println("EDP_MODE");

}
void loop()
{

}