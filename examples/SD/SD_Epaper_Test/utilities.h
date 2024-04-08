
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


#define I2C_SDA                 18
#define I2C_SCL                 17
#define OLED_RST                UNUSE_PIN

#define EDP_BUSY_PIN            48
#define EDP_RSET_PIN            47
#define EDP_DC_PIN              16
#define EDP_CS_PIN              15
#define EDP_CLK_PIN             14 // CLK
#define EDP_MOSI_PIN            11 // MOSI
#define EDP_MISO_PIN            -1

#define SDCARD_MOSI             EDP_MOSI_PIN
#define SDCARD_SCLK             EDP_CLK_PIN
#define SDCARD_MISO             2
#define SDCARD_CS               13

// #define HAS_DISPLAY










