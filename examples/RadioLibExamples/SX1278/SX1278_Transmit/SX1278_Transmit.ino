/*
   RadioLib SX1278 Transmit Example

   This example transmits packets using SX1278 LoRa radio module.
   Each packet contains up to 256 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/


#include <RadioLib.h>
#include "boards.h"

SX1278 radio = new Module(RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);

    // initialize SX1278 with default settings
    Serial.print(F("[SX1278] Initializing ... "));
#ifndef LoRa_frequency
    int state = radio.begin(433.0);
#else
    int state = radio.begin(LoRa_frequency);
#endif

    if (state != RADIOLIB_ERR_NONE)
    {
        display.setRotation(1);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 15);
        display.println("Initializing: FAIL!");
        display.update();
    }

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
        radio.setOutputPower(17);
        radio.setBandwidth(125);
        radio.setCurrentLimit(120);
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }
}

void loop()
{
    Serial.print(F("[SX1278] Transmitting packet ... "));

    // you can transmit C-string or Arduino string up to
    // 256 characters long
    // NOTE: transmit() is a blocking method!
    //       See example SX127x_Transmit_Interrupt for details
    //       on non-blocking transmission method.
    int state = radio.transmit("Hello World!");

    // you can also transmit byte array up to 256 bytes long
    /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
    */

    if (state == RADIOLIB_ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F(" success!"));

        // print measured data rate
        Serial.print(F("[SX1278] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));

        display.setRotation(1);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 15);
        display.println("Transmitting: OK!");
        display.update();
    } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));

    } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
        // timeout occurred while transmitting packet
        Serial.println(F("timeout!"));

    } else {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);

    }

    // wait for a second before transmitting again
    delay(1000);
}


