#include "AVR.h"

uint64_t AVR::cycles = 0;

AVR::AVR() {
    bus = new BUS();
    flash = new PMEM(bus);
    io = new IO();
    cpu = new CPU(bus, flash, io);

    this->reset();
    this->freq = 1000000; //1Mhz default
}

AVR::~AVR() {
   delete bus;
   delete flash;
   delete cpu;
}

void AVR::cycle() {
    cpu->fetch();
    cpu->decode();
    cpu->execute();
    cycles++;
}

void AVR::reset() {
    cycles = 0;
    cpu->reset();
    flash->erase();
    running = false;
}

void* AVR::helperRunThread(void *data) {
    AVR *avr = static_cast<AVR*>(data);
    avr->loop();
    return NULL;
}

void AVR::run() {
    running = true;
    pthread_create(&th_clock, NULL, helperRunThread, static_cast<void*>(this));
}

void AVR::loop() {
    while (running) {
        this->cycle();
        delayMicroSeconds(1000000/freq);
    }
    return;
}

void AVR::delayMicroSeconds(float microseconds) {
    __int64 timeEllapsed;
    __int64 timeStart;
    __int64 timeDelta;

    QueryPerformanceFrequency( (LARGE_INTEGER*)(&timeDelta ) );

    __int64 timeToWait = (double)timeDelta * (double)microseconds / 1000000.0f;

    QueryPerformanceCounter ( (LARGE_INTEGER*)(&timeStart ) );

    timeEllapsed = timeStart;

    while( ( timeEllapsed - timeStart ) < timeToWait ) {
        QueryPerformanceCounter( (LARGE_INTEGER*)(&timeEllapsed ) );
    };
}


void AVR::stop() {
    running = false;
}

void AVR::moreOneCycle() {
   cycles++;
}

uint64_t AVR::getCycles() {
    return cycles;
}

void AVR::setFreq(uint32_t freq) {
    this->freq = freq;
}

uint32_t AVR::getFreq() {
    return freq;
}



