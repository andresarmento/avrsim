#include "PORT.h"

PORT::PORT(IO* io) {
    this->io = io;
}

PORT::~PORT() {

}

void PORT::setRegisters(uint8_t pin_addr, uint8_t ddr_addr, uint8_t port_addr, uint8_t pud_addr) {
    //Points to registers, based on address
    this->pin  = io->getRegPtr(pin_addr);
    this->ddr  = io->getRegPtr(ddr_addr);
    this->port = io->getRegPtr(port_addr);
    this->pur  = io->getRegPtr(pud_addr);
}

void PORT::getPortState(int state[8]) {
    for (int bitpos=0; bitpos<8; bitpos++) {
        state[bitpos] = getPinState(bitpos);
    }
}

int PORT::getPinState(uint8_t bitpos) {
    int pudbit = CHECK_BIT(*pur, 4); //PUD is bit 4 in MCUCR register

    //check if ddr bit is set (OUTPUT)
    if (CHECK_BIT(*ddr, bitpos)) {
        //port has output pin state
        if (CHECK_BIT(*port, bitpos))
            return IO_HIGH;
        else
            return IO_LOW;
    } else {
        //Check if pull-up is enabled
        if (CHECK_BIT(*port, bitpos) && !pudbit) {
            return IO_HIGH;
        } else {
            return IO_HIZ;
        }
    }
}

int PORT::setPortState(int state[8]) {
    int error = 0;
    for (int bitpos=0; bitpos<8; bitpos++) {
        if(setPinState(bitpos, state[bitpos])) error = 1;
    }
    return error;
}

int PORT::setPinState(uint8_t bitpos, int state) {
    int pudbit = CHECK_BIT(*pur, 4); //PUD is bit 4 in MCUCR register

    //check if ddr bit is set (OUTPUT)
    if (CHECK_BIT(*ddr, bitpos)) {
        //Do not touch in PIN
        if (state == IO_HIZ) {
            return 0; //No Error
        } else {
            return 1; //Port is configured as OUTPUT
        }
    } else {
        if (state == IO_HIZ) {
            //Check if pull-up is enabled
            if (CHECK_BIT(*port, bitpos) && !pudbit) {
                SET_BIT(*pin, bitpos);
            } else {
                //set pin bit = 0, 1, aleatorio ou erro
                CLEAR_BIT(*pin, bitpos);
            }
        } else {
            if (state)
                SET_BIT(*pin, bitpos);
            else
                CLEAR_BIT(*pin, bitpos);
        }
        return 0; //No error
    }
}

