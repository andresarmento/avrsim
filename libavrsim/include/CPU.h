#ifndef CPU_H
#define CPU_H

#include <GPR.h>
#include <SREG.h>
#include <ALU.h>
#include <BUS.h>
#include <PMEM.h>
#include <IO.h>

enum ID_ERRORS {
    NO_ERROR = 0,    // No Error
    ERR_INSTRUCTION, // Instruction no supported or no implemented
};

class CPU {
    public:
        ALU*  alu;    // ALU

        void setPC(uint16_t val);
        uint16_t getPC();
        void setRI(uint16_t val);
        uint16_t getRI();
        uint8_t  getReg(uint8_t num);
        uint16_t  getRegX();
        uint16_t  getRegY();
        uint16_t  getRegZ();

        SREG getStatus();
        uint16_t fetch();
        uint16_t decode();
        uint16_t execute();
        void reset();

        CPU(BUS* bus, PMEM* flash, IO* io);
        virtual ~CPU();
    private:
        SREG  sreg;   // Program Status Register
        GPR    gpr;   // General Purpose Working Registers
        BUS*  bus;    // Bus
        PMEM* flash;  // Program Memory
        IO*  io  ;    // I/O
        uint16_t pc;  // Program Counter
        uint16_t ri;  // Instruction Register
};

#endif // CPU_H
