#ifndef ALU_H
#define ALU_H

#include <GPR.h>
#include <SREG.h>
#include <BUS.h>

typedef union {
    uint8_t byte;
    struct {
        unsigned b0 : 1;
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
    };
} AluOper;

class ALU {
    public:
        uint8_t execute(uint8_t cmd);
        void  setRdAddr(uint8_t addr);
        void  setRrAddr(uint8_t addr);
        void  setK(uint8_t val);

        ALU(GPR* gpr, SREG* sreg);
        virtual ~ALU();
    private:
        uint8_t RdAddr;
        uint8_t RrAddr;
        AluOper Rd;  //ALU input operator Rd
        AluOper Rr;  //ALU input operator Rr
        AluOper R;   //ALU output
        AluOper Rdh; //ALU input operator Rdh
        uint8_t K;   //ALU immediate operator

        GPR*  gpr;
        SREG* sreg;
};

#endif // ALU_H
