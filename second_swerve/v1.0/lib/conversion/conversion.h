#pragma once
#include <stdio.h>

namespace Conversion {
    struct Send_Data {
        uint8_t High_Byte;
        uint8_t Low_Byte;
       void update(int16_t Strength);
    };

    struct Available_Data {
        int16_t Become(double Current);
        int16_t Become_Angle(double Current); 
        private:
            int16_t Result;
            double calc;
    };
} // namespace Conversion
