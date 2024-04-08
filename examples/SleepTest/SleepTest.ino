
//This program only tests T3_V1.6.1
#include <RadioLib.h>
#include "boards.h"


SX1262 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.println("sleep test");

    Serial.print(F("[SX1262] Initializing ... "));
    int state = radio.begin(LoRa_frequency);
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println(F("success!"));
    }
    else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }

    delay(2000);
    display.powerDown();
    // u8g2->sleepOn();
    radio.sleep();

    SPI.end();
    SDSPI.end();

    pinMode(RADIO_CS_PIN, INPUT);
    pinMode(RADIO_RST_PIN, INPUT);
    pinMode(RADIO_DIO0_PIN, INPUT);
    pinMode(RADIO_CS_PIN, INPUT);
    pinMode(I2C_SDA, INPUT);
    pinMode(I2C_SDA, INPUT);
    pinMode(I2C_SCL, INPUT);
    pinMode(OLED_RST, INPUT);
    pinMode(RADIO_SCLK_PIN, INPUT);
    pinMode(RADIO_MISO_PIN, INPUT);
    pinMode(RADIO_MOSI_PIN, INPUT);
    pinMode(SDCARD_MOSI, INPUT);
    pinMode(SDCARD_MISO, INPUT);
    pinMode(SDCARD_SCLK, INPUT);
    pinMode(SDCARD_CS, INPUT);
    pinMode(BOARD_LED, INPUT);
    // pinMode(BAT_ADC_PIN, INPUT);

    delay(2000);

    esp_sleep_enable_timer_wakeup(30 * 1000 * 1000);
    esp_deep_sleep_start();
    
}

void loop()
{
}
