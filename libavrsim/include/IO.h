#ifndef IO_H
#define IO_H

#include <map>
#include <vector>
#include <string>
#include <inttypes.h>
#include <PORT.h>
#include <CFG.h>

#define CHECK_BIT(reg, bitpos) ((reg) & (0x01<<(bitpos)))
#define   SET_BIT(reg, bitpos) (reg = reg | (0x01<<(bitpos)))
#define CLEAR_BIT(reg, bitpos) (reg = reg & ~(0x01<<(bitpos)))

using namespace std;

class PORT; //Forward declaration

typedef map<string, PORT*> mapPORT;

//I/O Registers
class IO {
    public:
        IO();
        virtual ~IO();

        PORT* Port(string portname);
        void setReg(uint8_t addr, uint8_t bitpos);
        void clrReg(uint8_t addr, uint8_t bitpos);
        uint8_t* getRegPtr(uint8_t addr);
    private:
        uint8_t reg[64];    //IO Registers
        mapPORT ports;      //Digital Ports
};

/*
enum IO_MAP  { //0x20 - 0x5F

    //0x00 - 0x02 - Reserved

    PINB  = 0x03,  //The Port B Input Pins Address           (SRAM: 0x23)
    DDRB  = 0x04,  //The Port B Data Direction Register      (SRAM: 0x24)
    PORTB = 0x05,  //The Port B Data Register                (SRAM: 0x25)
    PINC  = 0x06,  //The Port C Input Pins Address           (SRAM: 0x26)
    DDRC  = 0x07,  //The Port C Data Direction Register      (SRAM: 0x27)
    PORTC = 0x08,  //The Port C Data Register                (SRAM: 0x28)
    PIND  = 0x09,  //The Port D Input Pins Address           (SRAM: 0x29)
    DDRD  = 0x0A,  //The Port D Data Direction Register      (SRAM: 0x2A)
    PORTD = 0x0B,  //The Port D Data Register                (SRAM: 0x2B)

    //0x0C - 0x14 - Reserved

    TIFR0 = 0x15,  //Timer/Counter 0 Interrupt Flag Register (SRAM: 0x35)
    TIFR1 = 0x16,  //Timer/Counter 1 Interrupt Flag Register (SRAM: 0x36)
    TIFR2 = 0x17,  //Timer/Counter 2 Interrupt Flag Register (SRAM: 0x37)

    //0x18 - 0x1A - Reserved

    PCIFR = 0x1B,  // (SRAM: 0x3B)
    EIFR  = 0x1C,  // (SRAM: 0x3C)
    EIMSK = 0x1D,  // (SRAM: 0x3D)
    GPIOR0= 0x1E,  // (SRAM: 0x3E)
    EECR  = 0x1F,  // (SRAM: 0x3F)
    EEDR  = 0x20,  // (SRAM: 0x40)
    EEARL = 0x21,  // (SRAM: 0x41)
    EEARH = 0x22,  // (SRAM: 0x42)
    GTCCR = 0x23,  // (SRAM: 0x43)
    TCCR0A= 0x24,  // (SRAM: 0x44)
    TCCR0B= 0x25,  // (SRAM: 0x45)
    TCNT0 = 0x26,  // (SRAM: 0x46)
    OCR0A = 0x27,  // (SRAM: 0x47)
    OCR0B = 0x28,  // (SRAM: 0x48)

    //0x29 - Reserved

    GPIOR1= 0x2A,  //                                        (SRAM: 0x4A)
    GPIOR2= 0x2B,  //                                        (SRAM: 0x4B)
    SPCR  = 0x2C,  //                                        (SRAM: 0x4C)
    SPSR  = 0x2D,  //                                        (SRAM: 0x4D)
    SPDR  = 0x2E,  //                                        (SRAM: 0x4E)

    //0x2F - Reserved

    ACSR  = 0x30,  //

    //0x31 - 0x32 - Reserved

    SMCR  = 0x33,  //                                        (SRAM: 0x53)
    MCUSR = 0x34,  //                                        (SRAM: 0x54)
    MCUCR = 0x35,  //MCU Control Register                    (SRAM: 0x55)

    //0x36 - Reserved

    SPMCSR= 0x37, // (SRAM: 0x57)

    //0x38 - 0x3C - Reserved

    SPL   = 0x3D,  //Stack Pointer Low                       (SRAM: 0x5D)
    SPH   = 0x3E,  //Stack Pointer High                      (SRAM: 0x5E)
    SREG_ = 0x3F,  //Status Register                         (SRAM: 0x5F)
};
*/

#endif // IO_H
