#ifndef SREG_H
#define SREG_H

//Program Status Register
typedef struct {
    unsigned C : 1; // Zero
    unsigned Z : 1; // Carry
    unsigned N : 1; // Negative
    unsigned V : 1; // Overflow
    unsigned S : 1; // Sign
    unsigned H : 1; // Half carry (BCD arithmetic)
    unsigned T : 1; // Flag bit
    unsigned I : 1; // Global interrupt enable
} SREG;

#endif // SREG_H
