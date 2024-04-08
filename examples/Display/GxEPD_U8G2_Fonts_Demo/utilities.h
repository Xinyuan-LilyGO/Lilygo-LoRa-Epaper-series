
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



#if defined(LILYGO_S3_E_PAPER_V_1_0)

#define EDP_BUSY_PIN            48
#define EDP_RSET_PIN            47
#define EDP_DC_PIN              16
#define EDP_CS_PIN              15
#define EDP_CLK_PIN             14 // CLK
#define EDP_MOSI_PIN            11 // MOSI
#define EDP_MISO_PIN            -1


#else
#error "For the first use, please define the board version and model in <utilities. h>"
#endif









