#include "GPR.h"

GPR::GPR(BUS* bus) {
    this-> bus = bus;
}

GPR::~GPR() {
}

uint8_t GPR::getReg(uint8_t num) {
    if (num >= 0 && num <= 31)
        return this->r[num];
    else
        return 0;
}

void GPR::setReg(uint8_t num, uint8_t value) {
    if (num >= 0 && num <= 31)
        this->r[num] = value;
}

