#include "CPU.h"
#include <AVR.h>

CPU::CPU(BUS* bus, PMEM* flash, IO* io) {
    this->flash = flash;
    this->bus = bus;
    this->io = io;

    alu = new ALU(&gpr, &sreg);

    this->reset();
}

CPU::~CPU(){
    delete alu;
}

void CPU::setPC(uint16_t val) {
    this->pc = val;
}

uint16_t CPU::getPC() {
    return this->pc;
}

void CPU::setRI(uint16_t val) {
    this->ri = val;
}

uint16_t CPU::getRI() {
    return this->ri;
}

uint8_t CPU::getReg(uint8_t num) {
    return this->gpr.reg[num];
}

uint16_t CPU::getRegX() {
    return this->gpr.X;
}

uint16_t CPU::getRegY() {
    return this->gpr.Y;
}

uint16_t CPU::getRegZ() {
    return this->gpr.Z;
}

SREG CPU::getStatus() {
    return sreg;
}

uint16_t CPU::fetch() {
    //PC points to next instruction
    this->ri = flash->getData(this->pc);
    return 0;
}

uint16_t CPU::execute() {
    if (this->bus->ctrl != ALU_NOP) {
        this->alu->execute(bus->ctrl);
    }
    return 0;
}

void CPU::reset() {
    this->pc = 0;
    this->ri = 0;

    for (int k=0; k < 32; k++) {
        gpr.reg[k] = 0x00;
    }

    sreg.C = 0;
    sreg.Z = 0;
    sreg.N = 0;
    sreg.V = 0;
    sreg.S = 0;
    sreg.H = 0;
    sreg.T = 0;
    sreg.I = 0;
}

uint16_t CPU::decode() {
    //131 Instructions
    uint8_t n1;
    uint8_t d, r, A, b;
    uint32_t K;

    uint16_t opcode = this->ri;

    //First six bits
    n1 = (opcode >> 10) & 0b00111111;

    switch (n1) {
        case 0b000000:
            //NOP - No operation. NOP (0000 0000 0000 0000)
            if (opcode == 0x00) {
                this->pc++;
                bus->ctrl = ALU_NOP;
                return NO_ERROR;
            }
            break;

         case 0b000011:
            //ADD - Add Without Carry. ADD Rd, Rr (0000 11rd dddd rrrr)
            d = ((opcode >> 4) & 0b0000000000011111);
            r = ((opcode >> 5) & 0b0000000000010000) | (opcode & 0b0000000000001111);
            this->pc++;
            alu->setRdAddr(d);  //Set operands
            alu->setRrAddr(r);  //Set operands
            bus->ctrl = ALU_ADD;  //Control ALU
            return NO_ERROR;
            break;

        case 0b000111:
            //ADC - Add With Carry. ADC Rd, Rr (0001 11rd dddd rrrr)
            d = ((opcode >> 4) & 0b0000000000011111);
            r = ((opcode >> 5) & 0b0000000000010000) | (opcode & 0b0000000000001111);
            this->pc++;
            alu->setRdAddr(d);  //Set operands
            alu->setRrAddr(r);  //Set operands
            bus->ctrl = ALU_ADC;  //Control ALU
            return NO_ERROR;
            break;

        case 0b100101:
            //INC - Increment. INC Rd (1001 010d dddd 0011)
            if ((opcode & 0b1111111000001111) == 0b1001010000000011) {
                d = ((opcode >> 4) & 0b0000000000011111);
                this->pc++;
                alu->setRdAddr(d);    //Set operands
                bus->ctrl = ALU_INC;  //Control ALU
                return NO_ERROR;
            }

            //JMP – Jump. JMP k (1001 010k kkkk 110k kkkk kkkk kkkk kkkk) 0 <= k < 4M
            if ((opcode & 0b1111111000001110) == 0b1001010000001100) {
                K = ((opcode >> 3) & 0b0000000000111110) | (opcode & 0b0000000000000001);
                this->pc++;
                this->fetch(); //Fetch the last word to ri.
                K = (K << 16) + this->ri;
                bus->ctrl = ALU_NOP;
                AVR::moreOneCycle();   //this instruction use 3 cycles
                AVR::moreOneCycle();
                this->setPC(K);         //Jump!
                return NO_ERROR;
            }
            break;

            //ADIW – Add Immediate to Word. ADIW Rd+1:Rd,K (1001 0110 KKdd KKKK)
            if ((opcode & 0b1111111100000000) == 0b1001011000000000) {
                d = ((opcode >> 4) & 0b0000000000000011) * 2 + 24;   //d -> {24,26,28,30}, 0 <= K <= 63
                K = ((opcode >> 2) & 0b0000000000110000) | (opcode & 0b0000000000001111);
                this->pc++;
                AVR::moreOneCycle();   //this instruction use 2 cycles
                alu->setRdAddr(d);     //Set operands
                alu->setK(K);          //Set Immediate
                bus->ctrl = ALU_ADIW;  //Control ALU
                return NO_ERROR;
            }
            break;

        case 0b100110:
            //SBI – Set Bit in I/O Register. SBI A,b (1001 1010 AAAA Abbb)
            if ((opcode & 0b1111111100000000) == 0b1001101000000000) {
                A = ((opcode >> 3) & 0b0000000000011111);    // 0 <= A <= 31, 0 <= b <= 7
                b = opcode & 0b0000000000000111;
                io->setReg(A, b);
                bus->ctrl = ALU_NOP;
                AVR::moreOneCycle();   //this instruction use 2 cycles
                this->pc++;
                return NO_ERROR;
            }
            break;

            //CBI – Clear Bit in I/O Register. CBI A,b (1001 1000 AAAA Abbb)
            if ((opcode & 0b1111111100000000) == 0b1001100000000000) {
                A = ((opcode >> 3) & 0b0000000000011111);    // 0 <= A <= 31, 0 <= b <= 7
                b = opcode & 0b0000000000000111;
                io->clrReg(A, b);
                bus->ctrl = ALU_NOP;
                AVR::moreOneCycle();   //this instruction use 2 cycles
                this->pc++;
                return NO_ERROR;
            }
            break;

        case 0b110000:
        case 0b110001:
        case 0b110010:
        case 0b110011:
            //RJMP – Relative Jump. RJMP k (1100 kkkk kkkk kkkk)
            K = opcode & 0b0000111111111111;
            bus->ctrl = ALU_NOP;
            AVR::moreOneCycle();   //this instruction use 2 cycles
            if (K < 2048) {
                this->setPC(this->pc + K + 1);         //Jump!
            } else {
                this->setPC(this->pc - (~K & 0xFFF));  //Jump!
            }
            return NO_ERROR;
            break;

        case 0b111000:
        case 0b111001:
        case 0b111010:
        case 0b111011:
            //LDI - Load Immediate.  LDI Rd, K (1110 KKKK dddd KKKK)
            d = ((opcode >> 4) & 0b0000000000001111) + 16;  //16 <= d <= 31, 0 <= K <= 255
            K = ((opcode >> 4) & 0b0000000011110000 ) | (opcode & 0b0000000000001111);
            this->pc++;
            //Move literal to register
            //this->gpr->setReg(d, K);
            this->gpr.reg[d] = K;
             //Move instructions dont use ALU
            bus->ctrl = ALU_NOP;
            return NO_ERROR;
    }

    return ERR_INSTRUCTION; //Error
}
