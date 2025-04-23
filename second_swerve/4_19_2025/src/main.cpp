#include "mbed.h"
#include "rtos.h"
#include "PID.h"
PID PD[2];
#include <math.h>
#include "conversion.h"
Conversion::Send_Data conv[4];
Conversion::Available_Data C_Data[4];

// CAN ピン設定（STM32F446RE 用）
CAN can(PA_11, PA_12); // 1 Mbps

Ticker cansend;
// Ticker mti;
CANMessage canMsgSend;
CANMessage canMsgReceive;

void send() {
    can.write(canMsgSend);
}

void send_current() {           
    // conv[0].update(C_Data[0].Become(0.2));  
    canMsgSend.data[0] = conv[0].High_Byte;
    canMsgSend.data[1] = conv[0].Low_Byte;
    canMsgSend.data[2] = 0;
    canMsgSend.data[3] = 0;
    canMsgSend.data[4] = 0;
    canMsgSend.data[5] = 0;
    canMsgSend.data[6] = 0;
    canMsgSend.data[7] = 0;
}


int time_m = 0;
void my_time() {
    time_m++; 
}

int main() {
    can.frequency(1000000);           // DJI ESCは1Mbps
    can.mode(CAN::Normal);
    canMsgSend.id = 0x200;            // DJI ESCの制御CAN ID（0x200）
    canMsgSend.len = 8;               // データ長8バイト（4台分）
    for (int i = 0; i < 8; i++) canMsgSend.data[i] = 0;
    cansend.attach(&send_current, 10ms);
    // mti.attach(&my_time, 1s);
    
    // int old = 0.0;
    int my_diff[2] = {0, 0};
    int comes = 0;
    int come = 0;
    double ansd = 0.0;

    while (true) {
        send();
        if (can.read(canMsgReceive)) {
            come = (canMsgReceive.data[0] << 8 | canMsgReceive.data[1]);
            int g = (canMsgReceive.data[2] << 8 | canMsgReceive.data[3]);
            double f = ((3.14 * 2.0) * (g / 60.0));
            my_diff[1] = (come - comes);
            if (my_diff[1] > (8192 / 2)) {
                my_diff[0]--;
            } else if (my_diff[1] < (-8192 / 2)) {
                my_diff[0]++;
            }
            ansd += (f * 0.01);
            double pd_pi = (PD[0].PD_move(3600.0, ansd)) / 3600.0;
            conv[0].update(C_Data[0].Become(pd_pi));  
            printf("a : %d, b : %lf, c : %lf\n", g, ansd, pd_pi);
            comes = come;
        }
        ThisThread::sleep_for(10ms);
    }
}
