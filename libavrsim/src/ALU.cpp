#include "ALU.h"

ALU::ALU(GPR* gpr, SREG* sreg) {
    this->gpr = gpr;
    this->sreg = sreg;
    RdAddr = 0;
    RrAddr = 0;
}

ALU::~ALU() {

}

void ALU::setRdAddr(uint8_t addr) {
    this->RdAddr = addr;
}

void ALU::setRrAddr(uint8_t addr) {
    this->RrAddr= addr;
}

void ALU::setK(uint8_t val) {
    this->K = val;
}

uint8_t ALU::execute(uint8_t cmd) {
    uint16_t Rw;
    Rd.byte = gpr->reg[RdAddr];
    Rr.byte = gpr->reg[RrAddr];

    switch (cmd & 0x00FF){
        case ALU_NOP:
            break;

        case ALU_ADC:
            R.byte = Rd.byte + Rr.byte + sreg->C;                //Add Operation
            sreg->N = R.b7;                                      //Negative flag
            if (R.byte == 0x00) sreg->Z = 1; else sreg->Z = 0;   //Zero flag
            sreg->S = sreg->N ^ sreg->V;                         //Sign flag
            sreg->H = (Rd.b3 & Rr.b3) | (Rr.b3 & ~R.b3) | (Rd.b3 & ~R.b3); //Half carry flag
            sreg->V = (Rd.b7 & Rr.b7 & ~R.b7) | (~Rd.b7 & ~Rr.b7 & R.b7);  //Overflow flag
            sreg->C = (Rd.b7 & Rr.b7) | (Rr.b7 & ~R.b7) | (~R.b7 & Rd.b7); //Carry flag
            gpr->reg[RdAddr] = R.byte;                           //Write Back to Rd
            break;

        case ALU_ADD:
            R.byte = Rd.byte + Rr.byte;                          //Add Operation
            sreg->N = R.b7;                                      //Negative flag
            if (R.byte == 0x00) sreg->Z = 1; else sreg->Z = 0;   //Zero flag
            sreg->S = sreg->N ^ sreg->V;                         //Sign flag
            sreg->H = (Rd.b3 & Rr.b3) | (Rr.b3 & ~R.b3) | (Rd.b3 & ~R.b3); //Half carry flag
            sreg->V = (Rd.b7 & Rr.b7 & ~R.b7) | (~Rd.b7 & ~Rr.b7 & R.b7);  //Overflow flag
            sreg->C = (Rd.b7 & Rr.b7) | (Rr.b7 & ~R.b7) | (~R.b7 & Rd.b7); //Carry flag
            gpr->reg[RdAddr] = R.byte;                           //Write Back to Rd
            break;

        case ALU_ADIW:
            Rdh.byte = gpr->reg[RdAddr+1];
            Rw =  Rdh.byte*256 + Rd.byte;
            Rw = Rw + K;                                         //Add Operation
            sreg->N = (Rw >> 15) & 0x01;                         //Negative flag
            if (Rw == 0x00) sreg->Z = 1; else sreg->Z = 0;       //Zero flag
            sreg->C = ~sreg->N & Rdh.b7;                         //Carry flag
            sreg->V = sreg->N & ~Rdh.b7;                         //Overflow flag
            sreg->S = sreg->N ^ sreg->V;                         //Sign flag
            gpr->reg[RdAddr] = Rw & 0x00FF;                      //Write Back to Rd
            gpr->reg[RdAddr+1] = (Rw >> 8) & 0x00FF;             //Write Back to Rdh
            break;

        case ALU_INC:
            if (Rd.byte == 0x7F) sreg->V = 1; else sreg->V = 0;  //Overflow flag
            R.byte = Rd.byte + 1;                                //Inc operation
            sreg->N = R.b7;                                      //Negative flag
            if (R.byte == 0x00) sreg->Z = 1; else sreg->Z = 0;   //Zero flag
            sreg->S = sreg->N ^ sreg->V;                         //Sign flag
            gpr->reg[RdAddr] = R.byte;                           //Write Back to Rd
            break;
    }

    return 0;
}

