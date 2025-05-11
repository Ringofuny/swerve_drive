#ifndef millis_H
#define millis_H

#include "mbed.h"  // STM32のモデルに合わせて適宜変更

class MILLIS {
    public:
    MILLIS(){
        millis_counter = 0;
        ms.attach(callback(this, &MILLIS::counter), 1ms);
    }

    
    uint32_t millis() {
        return millis_counter;
    }
    
    private:
        volatile uint32_t millis_counter;
        Ticker ms;
        // millis()関数（Arduinoと同じ）
        void counter() {
            millis_counter++;
        }
};

#endif