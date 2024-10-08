#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include "utilities.h"
#include <WiFi.h> 

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
GxIO_Class io(SDSPI,  EDP_CS_PIN, EDP_DC_PIN,  EDP_RSET_PIN);
GxEPD_Class display(io, EDP_RSET_PIN, EDP_BUSY_PIN);

Ticker ledTicker;


void WIFI_test(void)
{
    Serial.println("Scannig WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();    
    display.setRotation(3);        
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(0, 55);
    display.println("Scannig WiFi...");
    display.update();
    delay(10);      
    int network = WiFi.scanNetworks();
    if(network == 0) {
        Serial.println("  No network found.");
        }else {
            Serial.print(network);
            Serial.println("  Network found.");
            for(int i = 0; i < network; ++i) {
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(WiFi.SSID(i));  // Get the SSID (name) of the network
                Serial.print(" (");
                Serial.print(WiFi.RSSI(i));  // Get the signal strength in dBm
                Serial.println(")");
                if (WiFi.SSID(i) == "ssid") { // Check if the current network has the specified name
                    Serial.println("WiFi network found");
                    Serial.print("wifi_channel: ");
                    Serial.println(WiFi.channel(i));  // Get the channel number of the network
        }
        }
    }
}
void EPD_init(void)
{
    display.init(); 
    display.setTextColor(GxEPD_BLACK);
    delay(10);
    display.setRotation(2);
    delay(10);
    display.fillScreen(GxEPD_WHITE);
    delay(10);
    display.drawExampleBitmap(LILYGO_logo, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
    display.update();
}
void SD_test(void)
{
    if (!SD.begin(SDCARD_CS, SDSPI))
    {
        Serial.println("setupSDCard FAIL");
        display.setRotation(3);        
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 55);
        display.println("SD init failed");
        display.update();

    }
    else{
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        Serial.print("setupSDCard PASS . SIZE = ");
        Serial.print(cardSize / 1024.0);
        Serial.println(" GB");

        display.setRotation(3);
        display.setCursor(0, 55);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.printf("SDCard insert:%.2f GB",SD.cardSize() / 1024.0 / 1024.0 / 1024.0);
        display.update();
        delay(100);
    }
}
void initBoard()
{
    Serial.begin(115200);
    Serial.println("initBoard");

    pinMode(RADIO_POW_PIN, OUTPUT);
    digitalWrite(RADIO_POW_PIN, LED_ON);

    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);

#ifdef BOARD_LED
    pinMode(BOARD_LED, OUTPUT);
    digitalWrite(BOARD_LED, LED_ON);
#endif

    pinMode(SDCARD_MISO, INPUT_PULLUP);
    SDSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);

    EPD_init();
    
    WIFI_test();

#ifdef HAS_SDCARD

    SD_test();
    delay(1000);

#endif
}
