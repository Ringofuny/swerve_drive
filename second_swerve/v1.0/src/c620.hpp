#pragma once
#include "mbed.h"

struct C620 {
    int16_t angle;

    int16_t get_angle(const CANMessage& msg) {
        if (msg.id >= 0x200 && msg.id <= 0x208 && msg.len == 8) {
            angle = (msg.data[0] << 8 | msg.data[1]); 
            return angle;
        }
    }
};