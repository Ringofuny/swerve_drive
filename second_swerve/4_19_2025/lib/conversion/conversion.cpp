#include "conversion.h"

void Conversion::Send_Data::update(int16_t Strength) {
    High_Byte = (Strength >> 8);
    Low_Byte = (Strength & (0x00FF));
}

int Conversion::Available_Data::Become(double Current)
{
    Result = ((Current / 20) * 16384);
    return Result;
}
