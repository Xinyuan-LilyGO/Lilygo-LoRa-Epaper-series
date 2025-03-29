#pragma once

/*
* This sample program only supports SX1262
* */
#define LILYGO_S3_E_PAPER_V_1_0
/*
* The default program uses 868MHz,
* if you need to change it,
* please open this note and change to the frequency you need to test
* */

#define LoRa_frequency      915.0
#define Bandwidth           125
#define OutputPower         22 
#define Currentlimit        140
#define SpreadingFactor     12
#define CodeRate            6

#if defined(LILYGO_S3_E_PAPER_V_1_0)

#define EDP_BUSY_PIN            48
#define EDP_RSET_PIN            47
#define EDP_DC_PIN              16
#define EDP_CS_PIN              15
#define EDP_CLK_PIN             14 // CLK
#define EDP_MOSI_PIN            11 // MOSI
#define EDP_MISO_PIN            -1

#define RADIO_SCLK_PIN          5
#define RADIO_MISO_PIN          3
#define RADIO_MOSI_PIN          6
#define RADIO_CS_PIN            7
#define RADIO_DIO1_PIN          33
#define RADIO_BUSY_PIN          34
#define RADIO_RST_PIN           8
#define RADIO_POW_PIN           35

//! SX1276/78 module only
#define RADIO_DIO0_PIN          9
#define RADIO_DIO3_PIN          21
#define RADIO_DIO4_PIN          10
#define RADIO_DIO5_PIN          36
//! end

#define SDCARD_MOSI             EDP_MOSI_PIN
#define SDCARD_SCLK             EDP_CLK_PIN
#define SDCARD_MISO             2
#define SDCARD_CS               13

#define BOARD_LED               37
#define LED_ON                  HIGH

#define BAT_ADC_PIN             1
#define BUTTON_PIN              0

#define HAS_SDCARD

#else
#error "For the first use, please define the board version and model in <utilities. h>"
#endif
