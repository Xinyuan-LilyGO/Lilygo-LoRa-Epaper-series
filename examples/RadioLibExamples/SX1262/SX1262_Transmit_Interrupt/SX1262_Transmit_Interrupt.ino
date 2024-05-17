/*
    RadioLib SX1276 Transmit Example
    This example transmits packets using SX1276 LoRa radio module.
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
int transmissionState = RADIOLIB_ERR_NONE;
// flag to indicate that a packet was sent
volatile bool transmittedFlag = false;
int count = 0;
// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void)
{
    // check if the interrupt is enabled
    if (!enableInterrupt) {
        return;
    }

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
    int state = radio.begin(LoRa_frequency);

    if (state == RADIOLIB_ERR_NONE) 
    {
        radio.setBandwidth(Bandwidth);
        radio.setOutputPower(OutputPower);
        radio.setCurrentLimit(Currentlimit);
        radio.setSpreadingFactor(SpreadingFactor);

        Serial.print("LoRa_frequency : ");
        Serial.println(LoRa_frequency);
        Serial.print("Bandwidth : ");
        Serial.println(Bandwidth);
        Serial.print("OutputPower : ");
        Serial.println(OutputPower);
        Serial.print("Currentlimit : ");
        Serial.println(Currentlimit);                
        Serial.print("SpreadingFactor : ");
        Serial.println(SpreadingFactor);  
        Serial.println(F("success!"));
    } else {       
        Serial.print(F("failed, code "));
        Serial.println(state);
        display.setRotation(3);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 15);
        display.println("Initializing: FAIL!");
        display.update();
        while (true);
    }

    // set the function that will be called
    // when packet transmission is finished
    radio.setDio1Action(setFlag);

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
        // disable the interrupt service routine while
        // processing the data
        enableInterrupt = false;

        // reset flag
        transmittedFlag = false;

        if (transmissionState == RADIOLIB_ERR_NONE) {
            // packet was successfully sent
            Serial.println(F("transmission finished!"));

            // NOTE: when using interrupt-driven transmit method,
            //       it is not possible to automatically measure
            //       transmission data rate using getDataRate()
            display.setRotation(1);
            display.fillScreen(GxEPD_WHITE);
            display.setTextColor(GxEPD_BLACK);
            display.setFont(&FreeMonoBold9pt7b);
            display.setCursor(0, 15);
            display.println("Transmitting: OK!");
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
        String str = "Hello ! #" + String(count++);
        transmissionState = radio.startTransmit(str);
        // you can also transmit byte array up to 256 bytes long
        /*
          byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                            0x89, 0xAB, 0xCD, 0xEF};
          int state = radio.startTransmit(byteArr, 8);
        */

        // we're ready to send more packets,
        // enable interrupt service routine
        enableInterrupt = true;
    }
}
