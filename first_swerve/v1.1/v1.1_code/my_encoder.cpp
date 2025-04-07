#include "my_encoder.h"

void menc::A_rise(DigitalIn &inputPin)
{
    if(inputPin == 1){
        enc_count--;
    }else{
        enc_count++;
    }
}

void menc::B_rise(DigitalIn &inputPin)
{
    if(inputPin == 1){
        enc_count++;
    }else{
        enc_count--;
    }
}

void menc::A_fall(DigitalIn &inputPin)
{
    if(inputPin == 1){
        enc_count++;
    }else{
        enc_count--;
    }
}

void menc::B_fall(DigitalIn &inputPin)
{
    if(inputPin == 1){
        enc_count--;
    }else{
        enc_count++;
    }
}
