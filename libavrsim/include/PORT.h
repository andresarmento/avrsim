#ifndef PORT_H
#define PORT_H

#include <IO.h>
#include <inttypes.h>

enum PINSTATE {
    IO_HIZ = -1,
    IO_LOW = 0,
    IO_HIGH = 1,
};

class IO; // forwawd declaration

class PORT {
    public:
        void getPortState(int state[8]);
        int  getPinState(uint8_t bitpos);
        int setPortState(int state[8]);
        int setPinState(uint8_t bitpos, int state);
        void setRegisters(uint8_t pin_addr, uint8_t ddr_addr, uint8_t port_addr, uint8_t pud_addr);
        PORT(IO *io);
        virtual ~PORT();
    private:
        IO *io;
        uint8_t* pin;
        uint8_t* ddr;
        uint8_t* port;
        uint8_t* pur;
};

#endif // PORT_H
