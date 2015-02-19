#ifndef AVR_H
#define AVR_H

#include <PMEM.h>
#include <CPU.h>
#include <BUS.h>
#include <IO.h>
#include <pthread.h>
#include <libxml/tree.h>

class AVR {
    public:
        PMEM* flash;
        CPU*  cpu;
        IO*   io;

        void cycle();
        void reset();
        void run();
        void stop();
        void loop();
        static void moreOneCycle();
        uint64_t getCycles();
        void setFreq(uint32_t freq);
        uint32_t getFreq();
        static void* helperRunThread(void *data);

        AVR();
        virtual ~AVR();

    private:
        void delayMicroSeconds(float microseconds);

        BUS*  bus;
        static uint64_t cycles;
        pthread_t  th_clock;
        bool running;
        uint32_t freq;
};

#endif // AVR_H
