#ifndef my_pin_h
#define my_pin_h
#include "mbed.h"
#include "my_encoder.h"

CAN can(PA_11, PA_12); // CANピンの設定

UnbufferedSerial uart(PC_10, PC_11, 38400);

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

/*------------------------------エンコーダーの関数１------------------------------*/
menc enc[3];
void counter1_a_rise(){
    enc[0].A_rise(enc1_b);
}

void counter1_a_fall(){
    enc[0].A_rise(enc1_b);
}

void counter1_b_rise(){
    enc[0].A_rise(enc1_a);
}

void counter1_b_fall(){
    enc[0].A_rise(enc1_a);
}
/*------------------------------エンコーダーの関数１------------------------------*/

/*------------------------------エンコーダーの関数２------------------------------*/
void counter2_a_rise(){
    enc[1].A_rise(enc2_b);
}

void counter2_a_fall(){
    enc[1].A_rise(enc2_b);
}

void counter2_b_rise(){
    enc[1].A_rise(enc2_b);
}

void counter2_b_fall(){
    enc[1].A_rise(enc2_b);
}
/*------------------------------エンコーダーの関数２------------------------------*/

/*------------------------------エンコーダーの関数３------------------------------*/
void counter3_a_rise(){
    enc[2].A_rise(enc3_b);
}

void counter3_a_fall(){
    enc[2].A_rise(enc3_b);
}

void counter3_b_rise(){
    enc[2].A_rise(enc3_b);
}

void counter3_b_fall(){
    enc[2].A_rise(enc3_b);
}
/*------------------------------エンコーダーの関数３------------------------------*/

void rise_fall() {
    swi1_a.rise(callback(counter1_a_rise));
    swi1_a.fall(callback(counter1_a_fall));
    swi1_b.rise(callback(counter1_b_rise));
    swi1_b.fall(callback(counter1_b_fall));

    swi2_a.rise(callback(counter2_a_rise));
    swi2_a.fall(callback(counter2_a_fall));
    swi2_b.rise(callback(counter2_b_rise));
    swi2_b.fall(callback(counter2_b_fall));

    swi3_a.rise(callback(counter3_a_rise));
    swi3_a.fall(callback(counter3_a_fall));
    swi3_b.rise(callback(counter3_b_rise));
    swi3_b.fall(callback(counter3_b_fall));
}

#endif
