
#pragma once


// According to the board, cancel the corresponding macro definition
#define LILYGO_S3_E_PAPER_V_1_0



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









