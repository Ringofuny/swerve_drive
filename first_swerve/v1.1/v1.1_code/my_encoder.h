#pragma once
#include "mbed.h"

class menc { // my encoder -> menc
    public:
        int enc_count = 0;
        void A_rise(DigitalIn &inputPin);
        void B_rise(DigitalIn &inputPin);
        void A_fall(DigitalIn &inputPin);
        void B_fall(DigitalIn &inputPin);
};