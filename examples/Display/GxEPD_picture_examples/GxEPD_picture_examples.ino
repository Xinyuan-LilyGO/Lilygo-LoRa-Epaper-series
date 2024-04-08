/**
 * * Ink screen picture display test
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
SPIClass SDSPI(HSPI);

#include GxEPD_BitmapExamples
GxIO_Class io(SDSPI, EDP_CS_PIN, EDP_DC_PIN, EDP_RSET_PIN);
GxEPD_Class display(io, EDP_RSET_PIN, EDP_BUSY_PIN);

void setup()
{
#if LED_ON == LOW
    gpio_hold_dis(GPIO_NUM_4);
#endif
    pinMode(BOARD_LED, OUTPUT);
    digitalWrite(BOARD_LED, LED_ON);

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
    display.drawExampleBitmap(gImage_pkq1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
    display.update(); 
    delay(1000);
    display.drawExampleBitmap(LILYGO_logo, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
    display.update();
    Serial.println("EDP_MODE");

}
void loop()
{

}