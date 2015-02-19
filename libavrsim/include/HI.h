#ifndef HI_H
#define HI_H

#include <AVR.h>
#include <CFG.h>

using namespace std;

enum UserFunctions {
    FN_NEXT,
    FN_LOAD,
    FN_RESET,
    FN_GOTO,
    FN_RUN,
    FN_STOP,
    FN_FREQ,
    FN_SHOW_REGS,
    FN_SHOW_STATUS,
    FN_SHOW_CYCLES,
    FN_SHOW_FREQ,
    FN_SHOW_PORT,
    FN_PORT_SET,
    FN_PORT_CLEAR,
    FN_PORT_HIZ,
};

class HI {
    public:
        AVR* avr;
        CFG* cfg;
        string command(uint8_t cmd, char* data);

        HI();
        virtual ~HI();
    private:
        char response[700];
};

#endif // HI_H
