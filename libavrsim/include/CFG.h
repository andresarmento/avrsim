#ifndef CFG_H
#define CFG_H

#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <inttypes.h>

//#define IN_LIBXML
#include <libxml/xpath.h>

using namespace std;

typedef map<string, uint8_t> MAPREGS;
typedef vector<string> VECTSTR;

enum AVRTYPE {
    CFG_AVR = 0,
    CFG_TINY = 1,
    CFG_MEGA = 2,
    CFG_XMEGA = 3
};

class CFG {
    public:
        void loadAvrXML(char* filename);
        int getFamily();
        VECTSTR getIOPorts();
        uint8_t getAddrIO(string reg);
        static CFG* getInstance();

    private:
        static CFG *instance;
        int family;
        MAPREGS ioRegs;
        VECTSTR ioPorts;

        CFG();
        xmlXPathObjectPtr findNode(xmlDocPtr doc, string searchstring);
};

#endif //CFG_H
