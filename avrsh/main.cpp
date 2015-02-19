#include <iostream>
#include <map>
#include <HI.h>

using namespace std;

void showHelp();
map<std::string, UserFunctions> createMapStrFunctions();

int main() {
    char line[100], cmd[100], param1[100], param2[100], cmdp1[100];
    HI* hi = new HI();


    map<std::string, UserFunctions> mapFunc = createMapStrFunctions();


    cout << "AVR Command Line Simulator 0.2\n" << endl;

    while (true) {
        strcpy (cmd, "");
        strcpy (param1, "");
        strcpy (param2, "");
        cout << "arvsh> ";
        cin.getline(line, 100);
        if (strlen(line)==0) continue;
        sscanf(line, "%s %s %s", cmd, param1, param2);
        snprintf(cmdp1, 100, "%s %s", cmd, param1);
        if (strcmp(cmd, "exit")==0 || strcmp(cmd, "quit")==0) break;
        if (strcmp(cmd, "help")==0 || strcmp(cmd, "h")==0) {
            showHelp();
            continue;
        }
        if (mapFunc.find(cmd) == mapFunc.end() && mapFunc.find(cmdp1) == mapFunc.end()) {
            cout << "Command not found. Use: help\n\n";
            continue;
        }

        if (strcmp(cmd, "show")==0 || strcmp(cmd, "s")==0) {
            snprintf(cmd, 100, "show %s", param1);
            cout << hi->command(mapFunc[cmd], param2);
            continue;
        }

        if (strcmp(cmd, "port")==0 || strcmp(cmd, "p")==0) {
            snprintf(cmd, 100, "port %s", param1);
            cout << hi->command(mapFunc[cmd], param2);
            continue;
        }

        cout << hi->command(mapFunc[cmd], param1);
    }


    delete hi;
}

void showHelp() {
    cout << "Supported commands:" << endl << endl;
    cout << "help, h        This help." << endl;
    cout << "next, n        Execute next instruction." << endl;
    cout << "               Syntax: next or next <num> for num instructions." << endl;
    cout << "show regs      Show registers." << endl;
    cout << "show cycles    Show number of cycles." << endl;
    cout << "show freq      Show clock frequency." << endl;
    cout << "               Syntax: show regs or show regs <num> for specific register." << endl;
    cout << "show status    Show status register." << endl;
    cout << "show port      Show port I/O register. Syntax: show port <port letter>." << endl;
    cout << "port set       Set pin. Syntax: port set <portletter> <bitpos>." << endl;
    cout << "port clr       Clear pin. Syntax: port clr <portletter> <bitpos>." << endl;
    cout << "port hiz       Set pin. Syntax: port set <portletter> <bitpos>." << endl;
    cout << "load, l        Load hex file (Intel format). Syntax: load <file>." << endl;
    cout << "goto, g        Jump to address. Syntax: goto <hexaddr>." << endl;
    cout << "freq, f        Set clock frequency. Syntax: freq <val>." << endl;
    cout << "run            Run code (normal processing)." << endl;
    cout << "stop           Stop processing." << endl;
    cout << "reset          Reset MCU."  << endl;
    cout << "exit, quit     Terminate simulation and close program." << endl << endl;
}

map<std::string, UserFunctions> createMapStrFunctions() {
    map<std::string, UserFunctions> mapFunc;

    mapFunc["next"] = FN_NEXT;
    mapFunc["n"] = FN_NEXT;
    mapFunc["load"] = FN_LOAD;
    mapFunc["l"] = FN_LOAD;
    mapFunc["reset"] = FN_RESET;
    mapFunc["goto"] = FN_GOTO;
    mapFunc["g"] = FN_GOTO;
    mapFunc["run"] = FN_RUN;
    mapFunc["stop"] = FN_STOP;
    mapFunc["freq"] = FN_FREQ;
    mapFunc["f"] = FN_FREQ;

    mapFunc["show regs"] = FN_SHOW_REGS;
    mapFunc["show status"] = FN_SHOW_STATUS;
    mapFunc["show cycles"] = FN_SHOW_CYCLES;
    mapFunc["show freq"] = FN_SHOW_FREQ;
    mapFunc["show port"] = FN_SHOW_PORT;
    mapFunc["port set"] = FN_PORT_SET;
    mapFunc["port clr"] = FN_PORT_CLEAR;
    mapFunc["port hiz"] = FN_PORT_HIZ;
    mapFunc["s regs"] = FN_SHOW_REGS;
    mapFunc["s status"] = FN_SHOW_STATUS;
    mapFunc["s cycles"] = FN_SHOW_CYCLES;
    mapFunc["s freq"] = FN_SHOW_FREQ;
    mapFunc["s port"] = FN_SHOW_PORT;
    mapFunc["p set"] = FN_PORT_SET;
    mapFunc["p clr"] = FN_PORT_CLEAR;
    mapFunc["p hiz"] = FN_PORT_HIZ;

    return mapFunc;
}



