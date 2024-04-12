/**
 * @file      SDcard test.ino
 * @author    Puppy Liang (puppy0612@163.com)
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

void setup()
{

    Serial.begin(115200);

    pinMode(SDCARD_MISO, INPUT_PULLUP);
    SPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);

    if(!SD.begin(SDCARD_CS,SPI)){
        Serial.println("SD init failed");
        return;
    }else {
        Serial.println("SD init success");
        Serial.printf("SD Card insert:%.2f GB",SD.cardSize() / 1024.0 / 1024.0 / 1024.0);
    }
}
void loop()
{

}