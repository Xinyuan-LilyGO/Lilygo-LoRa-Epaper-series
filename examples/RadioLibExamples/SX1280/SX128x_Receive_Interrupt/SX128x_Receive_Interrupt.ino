/*
   RadioLib SX128x Receive with Interrupts Example

   This example listens for LoRa transmissions and tries to
   receive them. Once a packet is received, an interrupt is
   triggered. To successfully receive data, the following
   settings have to be the same on both transmitter
   and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word

   Other modules from SX128x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx128x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/




#include <RadioLib.h>
#include "boards.h"

SX1280 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

// flag to indicate that a packet was received
volatile bool receivedFlag = false;
static String rssi = "0dBm";
static String snr = "0dB";
static String payload = "0";
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
    // When the power is turned on, a delay is required.
    delay(1500);

    // initialize SX1280 with default settings
    Serial.print(F("[SX1280] Initializing ... "));
    int state = radio.begin();

    if (state != RADIOLIB_ERR_NONE) {
        display.setRotation(1);
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

#if defined(RADIO_RX_PIN) && defined(RADIO_TX_PIN)
    //Set ANT Control pins
    radio.setRfSwitchPins(RADIO_RX_PIN, RADIO_TX_PIN);
#endif


#ifdef LILYGO_T3_S3_V1_0
    // T3 S3 V1.1 with PA Version Set output power to 3 dBm    !!Cannot be greater than 3dbm!!
    int8_t TX_Power = 3;
#else
    // T3 S3 V1.2 (No PA) Version Set output power to 3 dBm    !!Cannot be greater than 3dbm!!
    int8_t TX_Power = 13;
#endif
    if (radio.setOutputPower(TX_Power) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Serial.println(F("Selected output power is invalid for this module!"));
        while (true);
    }

    // set carrier frequency to 2410.5 MHz
    if (radio.setFrequency(2400.0) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
        while (true);
    }

    // set bandwidth to 203.125 kHz
    if (radio.setBandwidth(203.125) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Serial.println(F("Selected bandwidth is invalid for this module!"));
        while (true);
    }

    // set spreading factor to 10
    if (radio.setSpreadingFactor(12) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        Serial.println(F("Selected spreading factor is invalid for this module!"));
        while (true);
    }

    // set coding rate to 6
    if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        Serial.println(F("Selected coding rate is invalid for this module!"));
        while (true);
    }
    // set the function that will be called
    // when packet transmission is finished
    radio.setDio1Action(setFlag);

    // start listening for LoRa packets
    Serial.print(F("[SX1280] Starting to listen ... "));
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }

}


void loop()
{
    // check if the flag is set
    if (receivedFlag) {

        // reset flag
        receivedFlag = false;

        // you can read received data as an Arduino String
        // String str;
        // int state = radio.readData(str);

        int state = radio.readData(payload);

        // you can also read received data as byte array
        /*
        byte byteArr[8];
        int state = radio.readData(byteArr, 8);
        */

        if (state == RADIOLIB_ERR_NONE) {

            rssi = String(radio.getRSSI()) + "dBm";
            snr = String(radio.getSNR()) + "dB";

            // packet was successfully received
            Serial.println(F("[SX1280] Received packet!"));

            // print data of the packet
            Serial.print(F("[SX1280] Data:\t\t"));
            Serial.println(payload);

            // print RSSI (Received Signal Strength Indicator)
            Serial.print(F("[SX1280] RSSI:\t\t"));
            Serial.println(rssi);

            // print SNR (Signal-to-Noise Ratio)
            Serial.print(F("[SX1280] SNR:\t\t"));
            Serial.println(snr);


            display.setRotation(3);
            display.fillScreen(GxEPD_WHITE);
            display.setTextColor(GxEPD_BLACK);
            display.setFont(&FreeMonoBold9pt7b);
            display.setCursor(0, 15);
            display.println("[SX128x] Received:");
            
            display.setCursor(0, 35);
            display.println("DATA:"); 
            display.setCursor(55, 35);
            display.println(payload); 

            display.setCursor(0, 55);
            display.println("RSSI:"); 
            display.setCursor(55, 55);
            display.println(rssi);

            display.setCursor(0, 75);
            display.println("SNR :"); 
            display.setCursor(55, 75);
            display.println(snr);  
            display.update();

        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            // packet was received, but is malformed
            Serial.println(F("[SX1280] CRC error!"));

        } else {
            // some other error occurred
            Serial.print(F("[SX1280] Failed, code "));
            Serial.println(state);

        }

        // put module back to listen mode
        radio.startReceive();

    }
}


