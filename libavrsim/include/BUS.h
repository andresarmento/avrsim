#ifndef BUS_H
#define BUS_H

#include <inttypes.h>

enum ALU_CTRL {
    ALU_NOP  = 0x00,
    ALU_ADC  = 0x01,
    ALU_ADD  = 0x02,
    ALU_ADIW = 0x03,
    ALU_INC  = 0x04,
};


typedef struct {
    uint8_t  data;
    uint16_t addr;
    uint16_t  ctrl;
} BUS;

#endif // BUS_H
