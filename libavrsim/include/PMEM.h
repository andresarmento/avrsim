#ifndef PMEM_H
#define PMEM_H

#define ATMEGA328P_PROGMEMSIZE 16384

#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <BUS.h>

using namespace std;

//Program Memory
class PMEM {
    public:
        void setData(uint16_t addr, uint16_t data);
        uint16_t getData(uint16_t addr);
        int readHex(char* filename);
        void erase();
        uint16_t getMinAddr();

        PMEM(BUS* bus);
        virtual ~PMEM();
    private:
        uint16_t minAddr;
        uint16_t maxAddr;
        BUS *bus;
        uint16_t data[ATMEGA328P_PROGMEMSIZE];  //16K x 16 bits
        int parseHexLine(char *theline, int bytes[], uint16_t* addr, unsigned int* num, int*code);
};

#endif // PMEM_H
