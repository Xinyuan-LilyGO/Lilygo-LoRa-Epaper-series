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

// flag to indicate that a packet was received
static volatile bool receivedFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void)
{
    // we got a packet, set the flag
    receivedFlag = true;
}

void setup()
{

    initBoard();
    Serial.println("[SX1262] Receive ");
    // When the power is turned on, a delay is required.
    delay(1500);
    // initialize SX1262 with default settings
    Serial.println(F("[SX1262] Initializing ... "));
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

    // set the function that will be called
    // when new packet is received
    radio.setDio1Action(setFlag);

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
    
    // set sync word
    Serial.println(F("[SX1262] Setting sync word ... "));
    state = radio.setSyncWord(0xAB);
    if (state != RADIOLIB_ERR_NONE) {
        Serial.println(F("Unable to set sync word!"));
        while (true);
    }
    
    // set current limit
    Serial.println(F("[SX1262] Setting current limit ... "));
    state = radio.setCurrentLimit(140);
    if (state == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
        Serial.println(F("Selected current limit is invalid for this module!"));
        while (true);
    }
    
    // set preamble length
    Serial.println(F("[SX1262] Setting preamble length ... "));
    state = radio.setPreambleLength(16);
    if (state == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
        Serial.println(F("Selected preamble length is invalid for this module!"));
        while (true);
    }
    
    // set CRC
    Serial.println(F("[SX1262] Setting CRC ... "));
    state = radio.setCRC(false);
    if (state == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
        Serial.println(F("Selected CRC is invalid for this module!"));
        while (true);
    }
    
    // DIO2 as RF switch - critical for SX1262 modules
    Serial.println(F("[SX1262] Setting DIO2 as RF switch ... "));
    state = radio.setDio2AsRfSwitch();
    if (state != RADIOLIB_ERR_NONE) {
        Serial.println(F("Failed to set DIO2 as RF switch!"));
        while (true);
    }

    // start listening for LoRa packets
    Serial.println(F("[SX1262] Starting to listen ... "));
    state = radio.startReceive();

    if (state != RADIOLIB_ERR_NONE)
    {
        display.setRotation(3);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 15);
        display.println("listening: FAIL!");
        display.update();
        Serial.print(F("failed, code "));
        Serial.println(state);
    }
    else{
        Serial.println(F("success!"));
    }

    // if needed, 'listen' mode can be disabled by calling
    // any of the following methods:
    //
    // radio.standby()
    // radio.sleep()
    // radio.transmit();
    // radio.receive();
    // radio.readData();
    // radio.scanChannel();
}

void loop()
{
    // check if the flag is set
    if (receivedFlag)
    {

        // reset flag
        receivedFlag = false;

        // you can read received data as an Arduino String
        String str;
        int state = radio.readData(str);

        // you can also read received data as byte array
        /*
        byte byteArr[8];
        int state = radio.readData(byteArr, 8);
        */

        if (state == RADIOLIB_ERR_NONE)
        {
            // packet was successfully received
            Serial.println(F("[SX1262] Received packet!"));

            // print data of the packet
            Serial.print(F("[SX1262] Data:\t\t"));
            Serial.println(str);

            // print RSSI (Received Signal Strength Indicator)
            Serial.print(F("[SX1262] RSSI:\t\t"));
            Serial.print(radio.getRSSI());
            Serial.println(F(" dBm"));

            // print SNR (Signal-to-Noise Ratio)
            Serial.print(F("[SX1262] SNR:\t\t"));
            Serial.print(radio.getSNR());
            Serial.println(F(" dB"));

            display.setRotation(3);
            display.fillScreen(GxEPD_WHITE);
            display.setTextColor(GxEPD_BLACK);
            display.setFont(&FreeMonoBold9pt7b);
            display.setCursor(0, 15);
            display.println("[SX1262] Received:");
            display.setCursor(0, 35);
            display.println("DATA:");
            display.setCursor(55, 35);
            display.println(str);
            display.setCursor(0, 55);
            display.println("RSSI:"); 
            display.setCursor(55, 55);
            display.println(radio.getRSSI());
            display.setCursor(0, 75);
            display.println("SNR :"); 
            display.setCursor(55, 75);
            display.println(radio.getSNR());  
            display.update();

        }
        else if (state == RADIOLIB_ERR_CRC_MISMATCH)
        {
            // packet was received, but is malformed
            Serial.println(F("CRC error!"));
        }
        else
        {
            // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(state);
        }

        // put module back to listen mode
        radio.startReceive();

    }
}
