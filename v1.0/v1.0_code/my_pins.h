#ifndef my_pin_h
#define my_pin_h
#include "mbed.h"

DigitalOut led1(PB_14);
DigitalOut led2(PB_15);

DigitalIn enc1_a(PA_8);
DigitalIn enc1_b(PA_9);
DigitalIn enc2_a(PA_0);
DigitalIn enc2_b(PA_1);
DigitalIn enc3_a(PB_4);
DigitalIn enc3_b(PB_5);
InterruptIn swi1_a(PA_8);
InterruptIn swi1_b(PA_9);
InterruptIn swi2_a(PA_0);
InterruptIn swi2_b(PA_1);
InterruptIn swi3_a(PB_4);
InterruptIn swi3_b(PB_5);

PwmOut pwm1(PB_2);
PwmOut pwm2(PC_8);
PwmOut pwm3(PC_9);
PwmOut pwm4(PB_6);
PwmOut pwm5(PB_10);
PwmOut pwm6(PA_10);

DigitalOut  dig1(PB_8);
DigitalOut  dig2(PB_9);
DigitalOut  dig3(PA_5);
DigitalOut  dig4(PA_6);
DigitalOut  dig5(PA_7);
DigitalOut  dig6(PC_7);

DigitalIn limsw1(PA_4);
DigitalIn limsw2(PB_0);

int enc_count = 0;
int enc2_count = 0;
int enc3_count = 0;

float g_speed = 0;
float c_speed = 0;

struct SEND{
    uint8_t RX;
    uint8_t RY;
    uint8_t LY;
    uint8_t goback;
    uint8_t mode;
    uint8_t comp;
    uint8_t FAIL;
};
struct SEND send;

#endif
