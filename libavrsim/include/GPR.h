#ifndef GPR_H
#define GPR_H

#include <inttypes.h>
#include <BUS.h>

//General Purpose Working Registers
typedef union {
    uint8_t reg[32];
    struct {
        uint8_t dummy[26];
        uint16_t X;   //Special Addressing Registers
        uint16_t Y;
        uint16_t Z;
    };
} GPR;

#endif // GPR_H
