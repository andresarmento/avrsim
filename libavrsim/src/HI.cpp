#include "HI.h"

HI::HI() {
    avr = new AVR();
}

HI::~HI() {
    delete avr;
    CFG* cfg = CFG::getInstance();
    delete cfg;
}

string HI::command(uint8_t cmd, char* data) {
    uint32_t n;
    uint16_t addr = 0;
    char line[30];
    int state[8];
    char port[1];
    int bitpos;
    //CFG* cfg = CFG::getInstance();

    switch (cmd) {
        case FN_NEXT:
            if (strlen(data)) {
                n = atoi(data);
                for (uint32_t k=0; k < n; k++) {
                     addr = avr->cpu->getPC();
                     avr->cycle();
                }
            } else {
                addr = avr->cpu->getPC();
                avr->cycle();
            }

            snprintf(response, 100, "Executed: PC = 0x%04X, Instruction = 0x%04X\n", addr, avr->cpu->getRI());

            break;

        case FN_SHOW_REGS:
            if (strlen(data)) {
                n = atoi(data);
                if (n >=0 && n < 32) {
                    snprintf(response, 30, "R%d = 0x%02X (%d)\n", n, avr->cpu->getReg(n),  avr->cpu->getReg(n));
                } else {
                    strcpy(response, "ERROR: Register not exist!\n");
                }
            } else {
                strcpy (response, "All Registers:\n\n");
                for (int k=0; k < 32; k++) {
                    snprintf(line, 30, "R%02d = 0x%02X (%d)", k, avr->cpu->getReg(k),  avr->cpu->getReg(k));
                    if ((k+1)%4==0) strcat(line, "\n"); else strcat(line, "\t");
                    strncat(response, line, 700);
                }
                snprintf(line, 30, "X = 0x%04X (%d)\n", avr->cpu->getRegX(), avr->cpu->getRegX());
                strncat(response, line, 700);
                snprintf(line, 30, "Y = 0x%04X (%d)\n", avr->cpu->getRegY(), avr->cpu->getRegY());
                strncat(response, line, 700);
                snprintf(line, 30, "Z = 0x%04X (%d)\n", avr->cpu->getRegZ(), avr->cpu->getRegZ());
                strncat(response, line, 700);
            }
            break;

        case FN_SHOW_STATUS:
            snprintf(response, 100, "| I  T  H  S  V  N  Z  C |\n| %u  %u  %u  %u  %u  %u  %u  %u |\n",
                     avr->cpu->getStatus().I, avr->cpu->getStatus().T, avr->cpu->getStatus().H, avr->cpu->getStatus().S,
                     avr->cpu->getStatus().V, avr->cpu->getStatus().N, avr->cpu->getStatus().Z, avr->cpu->getStatus().C);
            break;

        case FN_SHOW_CYCLES:
            snprintf(response, 100, "Cycles = %I64d\n", avr->getCycles());
            break;

        case FN_SHOW_PORT:
            if (avr->io->Port(data)) {
                avr->io->Port(data)->getPortState(state);
                snprintf(response, 100, "| 7  6  5  4  3  2  1  0 |\n|");
                for (int k=7; k>=0; k--) {
                    if (state[k] == -1)
                        snprintf(line, 30, " Z ");
                    else
                        snprintf(line, 30, " %d ", state[k]);
                    strncat(response, line, 700);
                }
                strncat(response, "|\n", 700);
            } else {
                strcpy(response, "Port does not exist!\n");
            }
            break;

        case FN_LOAD:
            if (avr->flash->readHex(data)) {
                avr->cpu->setPC(avr->flash->getMinAddr()); // Set PC to min Addr found in Hex file
                snprintf(response, 100, "File: %s is OK!\n", data);
            } else {
                snprintf(response, 100, "Error opening file: %s.\n", data);
            }
            break;

        case FN_RESET:
            avr->reset();
            snprintf(response, 100, "MCU Reset.\n");
            break;

        case FN_GOTO:
            n = strtol(data, NULL, 16);
            avr->cpu->setPC(n);
            snprintf(response, 100, "PC (Program Counter) now is: 0x%02X.\n", n);
            break;

        case FN_RUN:
            avr->run();
            snprintf(response, 100, "Start simulation.\n");
            break;

        case FN_STOP:
            avr->stop();
            snprintf(response, 100, "Stop.\n");
            break;

        case FN_FREQ:
            if (strlen(data)) {
                n = atoi(data);
                if (n >=1 && n < 1000000) {
                    avr->setFreq(n);
                    snprintf(response, 100, "New clock frequency: %uHz\n", n);
                } else {
                    strcpy (response, "You have to specify a frequency between 1 and 1000000 (Hz).\n");
                }
            } else {
                strcpy (response, "You have to specify a frequency in Hertz.\n");
            }
            break;

        case FN_SHOW_FREQ:
            snprintf(response, 100, "Clock frequency: %uHz\n", avr->getFreq());
            break;

        case FN_PORT_SET:
            sscanf(data, "%s %d", port, &bitpos);
            if (bitpos >= 0 && bitpos <= 7) {
                if (avr->io->Port(port)) {
                    if (avr->io->Port(port)->setPinState(bitpos, IO_HIGH)) {
                        snprintf(response, 100, "Dangerous condition. Probably because this pin is an output!\n");
                    }
                } else {
                    strcpy(response, "Port does not exist!\n");
                }
            } else {
                snprintf(response, 100, "You have to specify a bit between 0 and 7.\n");
            }
            break;

        case FN_PORT_CLEAR:
            sscanf(data, "%s %d", port, &bitpos);
            if (bitpos >= 0 && bitpos <= 7) {
                if (avr->io->Port(port)) {
                    if (avr->io->Port(port)->setPinState(bitpos, IO_LOW)) {
                        snprintf(response, 100, "Dangerous condition. Probably because this pin is an output!\n");
                    }
                } else {
                    strcpy(response, "Port does not exist!\n");
                }
            } else {
                snprintf(response, 100, "You have to specify a bit between 0 and 7.\n");
            }
            break;

        case FN_PORT_HIZ:
            sscanf(data, "%s %d", port, &bitpos);
            if (bitpos >= 0 && bitpos <= 7) {
                if (avr->io->Port(port)) {
                    avr->io->Port(port)->setPinState(bitpos, IO_HIZ);
                } else {
                    strcpy(response, "Port does not exist!\n");
                }
            } else {
                snprintf(response, 100, "You have to specify a bit between 0 and 7.\n");
            }
            break;
    }

    return string(response);
}
