/*
    RadioLib SX1262 Transmit Example
    This example transmits packets using SX1262 LoRa radio module.
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

SX1262 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);


// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1262 radio = RadioShield.ModuleA;

// save transmission state between loops
static int transmissionState = RADIOLIB_ERR_NONE;
// flag to indicate that a packet was sent
volatile bool transmittedFlag = false;
static uint32_t count = 0;
static String payload;
// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void)
{
    // we sent a packet, set the flag
    transmittedFlag = true;
}

void setup()
{

    initBoard();
    Serial.println("[SX1262] Transmit ");
    // When the power is turned on, a delay is required.
    delay(1500);
           
    // initialize SX1262 with default settings
    Serial.print(F("[SX1262] Initializing ... "));
    int state = radio.begin();

    if (state != RADIOLIB_ERR_NONE) {
        display.setRotation(3);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 15);
        display.println("Initializing: FAIL!");
        display.update();
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }
    else {
        Serial.println(F("success!"));
    }

    // set carrier frequency 
    if (radio.setFrequency(LoRa_frequency) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
        while (true);
    }

    // set bandwidth 
    if (radio.setBandwidth(Bandwidth) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Serial.println(F("Selected bandwidth is invalid for this module!"));
        while (true);
    }

    // set spreading factor 
    // SX1262 :  Allowed values range from 5 to 12.
    if (radio.setSpreadingFactor(SpreadingFactor) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        Serial.println(F("Selected spreading factor is invalid for this module!"));
        while (true);
    }

    // set coding rate 
    // SX1262 : Allowed values range from 5 to 8. Only available in LoRa mode.
    if (radio.setCodingRate(CodeRate) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        Serial.println(F("Selected coding rate is invalid for this module!"));
        while (true);
    }

    // SX1262 :  Allowed values are in range from -9 to 22 dBm. This method is virtual to allow override from the SX1261 class.
    if (radio.setOutputPower(OutputPower) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Serial.println(F("Selected output power is invalid for this module!"));
        while (true);
    }

    // *** ADDED: CRITICAL SX1262 CONFIGURATIONS ***
    
    // set sync word - important for receiver/transmitter compatibility
    Serial.println(F("[SX1262] Setting sync word ... "));
    state = radio.setSyncWord(0xAB);
    if (state != RADIOLIB_ERR_NONE) {
        Serial.println(F("Unable to set sync word!"));
        while (true);
    }
    
    // set current limit for overcurrent protection
    Serial.println(F("[SX1262] Setting current limit ... "));
    state = radio.setCurrentLimit(140);
    if (state == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
        Serial.println(F("Selected current limit is invalid for this module!"));
        while (true);
    }
    
    // set preamble length for better transmission reliability
    Serial.println(F("[SX1262] Setting preamble length ... "));
    state = radio.setPreambleLength(16);
    if (state == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
        Serial.println(F("Selected preamble length is invalid for this module!"));
        while (true);
    }
    
    // set CRC configuration
    Serial.println(F("[SX1262] Setting CRC ... "));
    state = radio.setCRC(false);
    if (state == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
        Serial.println(F("Selected CRC is invalid for this module!"));
        while (true);
    }
    
    // DIO2 as RF switch - CRITICAL for SX1262 modules
    // This is essential for the RF switch between RX and TX paths
    Serial.println(F("[SX1262] Setting DIO2 as RF switch ... "));
    state = radio.setDio2AsRfSwitch();
    if (state != RADIOLIB_ERR_NONE) {
        Serial.println(F("Failed to set DIO2 as RF switch!"));
        while (true);
    }
    
    // set the function that will be called
    // when packet transmission is finished
    // RadioLib 6.x API uses setPacketSentAction instead of setDio1Action
    radio.setPacketSentAction(setFlag);

    // start transmitting the first packet
    Serial.print(F("[SX1262] Sending first packet ... "));

    // you can transmit C-string or Arduino string up to
    // 256 characters long
    transmissionState = radio.startTransmit("Hello World!");

    // you can also transmit byte array up to 256 bytes long
    /*
      byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                        0x89, 0xAB, 0xCD, 0xEF};
      state = radio.startTransmit(byteArr, 8);
    */
}

void loop()
{
    // check if the previous transmission finished
    if (transmittedFlag) {

        // reset flag
        transmittedFlag = false;

        // IMPORTANT: RadioLib 6.x requires finishTransmit() call
        // to clean up internal flags before next transmission
        radio.finishTransmit();

        payload = "T3-Epaper Hi #" + String(count++);

        if (transmissionState == RADIOLIB_ERR_NONE) {
            // packet was successfully sent
            Serial.println(F("transmission finished!"));
            // NOTE: when using interrupt-driven transmit method,
            //       it is not possible to automatically measure
            //       transmission data rate using getDataRate()
            display.setRotation(3);
            display.fillScreen(GxEPD_WHITE);
            display.setTextColor(GxEPD_BLACK);
            display.setFont(&FreeMonoBold9pt7b);
            display.setCursor(0, 15);
            display.println("Transmitting: OK!");
            display.setCursor(0, 35);
            display.println(payload);
            display.update();
        } else {
            Serial.print(F("failed, code "));
            Serial.println(transmissionState);
        }

        // wait a second before transmitting again
        delay(1000);

        // send another one
        Serial.print(F("[SX1262] Sending another packet ... "));

        // you can transmit C-string or Arduino string up to
        // 256 characters long
        
        transmissionState = radio.startTransmit(payload);
        // you can also transmit byte array up to 256 bytes long
        /*
          byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                            0x89, 0xAB, 0xCD, 0xEF};
          int state = radio.startTransmit(byteArr, 8);
        */

    }
}
