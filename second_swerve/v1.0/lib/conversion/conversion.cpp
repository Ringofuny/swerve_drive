#include "conversion.h"
#include <math.h>

void Conversion::Send_Data::update(int16_t Strength) {
    High_Byte = (Strength >> 8);
    Low_Byte = (Strength & (0x00FF));
}

int16_t Conversion::Available_Data::Become(double Current)
{
    calc = ((Current / 10.0) * 16384);
    calc = std::round(calc);
    Result = static_cast<int16_t>(calc);
    return Result;
}
