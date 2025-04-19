#pragma once
#include <stdio.h>

namespace Conversion {
    struct Send_Data {
        int8_t High_Byte;
        int8_t Low_Byte;
        void update(int16_t Strength);
    };

    struct Available_Data {
        int Become(double Current); 
        private:
            int Result;
    };
} // namespace Conversion
