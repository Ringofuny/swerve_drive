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
        private:
            int Result;
            double calc;
    };
} // namespace Conversion
