#include "IO.h"

IO::IO() {
    //Clear regs
    for (int k=0; k < 64; k++) {
        reg[k] = 0;
    }

    //Create PORTs
    CFG* cfg = CFG::getInstance();
    //Get the port names (letters) from XML description file
    VECTSTR ioPorts = cfg->getIOPorts();
    //Iterate for each port name
    for (vector<string>::iterator pname = ioPorts.begin() ; pname != ioPorts.end(); ++pname) {
        //Create new PORT object
        PORT* port = new PORT(this);
        //Set address of registers PINx, DDRx, PORTx e PUR (MCUCR), MCUCR = 0x35 (Only for AVR Mega)
        port->setRegisters(cfg->getAddrIO("PIN" + *pname), cfg->getAddrIO("DDR" + *pname), cfg->getAddrIO("PORT" + *pname), 0x35);
        //Puts PORT object on MAP, indexed by portname (letter).
        ports[*pname] = port;
    }
}

IO::~IO () {
    CFG* cfg = CFG::getInstance();
    VECTSTR ioPorts = cfg->getIOPorts();

    for (vector<string>::iterator pname = ioPorts.begin() ; pname != ioPorts.end(); ++pname) {
        delete ports[*pname];
    }
}

PORT* IO::Port(string portname) {
    return ports[portname];
}

uint8_t* IO::getRegPtr(uint8_t addr) {
    return &reg[addr];
}

void IO::setReg(uint8_t addr, uint8_t bitpos)  {
    if (bitpos >=0 && bitpos <=7) {
        //uint8_t mask = 0x01 << bitpos;
        //reg[addr] = reg[addr] | mask;
        SET_BIT(reg[addr], bitpos);
    }
}

void IO::clrReg(uint8_t addr, uint8_t bitpos)  {
    if (bitpos >=0 && bitpos <=7) {
        //uint8_t mask = ~(0x01 << bitpos);
        //reg[addr] = reg[addr] & mask;
        CLEAR_BIT(reg[addr], bitpos);
    }
}






