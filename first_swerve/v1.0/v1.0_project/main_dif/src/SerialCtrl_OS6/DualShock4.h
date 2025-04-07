#include "mbed.h"

namespace NITSC
{
    struct DualShock4
    {
        enum DataPos
        {
            LX,
            LY,
            RX,
            RY,
            L2,
            R2,
            BUTTON_0,
            BUTTON_1,
            SUM,
            DATA_N
        };

        // BUTTON_0 map
        static constexpr uint8_t UP       = 0x01;
        static constexpr uint8_t RIGHT    = 0x02;
        static constexpr uint8_t DOWN     = 0x04;
        static constexpr uint8_t LEFT     = 0x08;
        static constexpr uint8_t TRIANGLE = 0x10;
        static constexpr uint8_t CIRCLE   = 0x20;
        static constexpr uint8_t CROSS    = 0x40;
        static constexpr uint8_t SQUARE   = 0x80;
        
        // BUTTON_1 map
        static constexpr uint8_t L1       = 0x01;
        static constexpr uint8_t R1       = 0x02;
        static constexpr uint8_t L3       = 0x04;
        static constexpr uint8_t R3       = 0x08;
        static constexpr uint8_t SHARE    = 0x10;
        static constexpr uint8_t OPTIONS  = 0x20;
        static constexpr uint8_t PS       = 0x40;
        static constexpr uint8_t TOUCHPAD = 0x80;
    };
}
