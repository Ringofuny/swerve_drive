#include "mbed.h"
#include "rtos.h"
#include "conversion.h"
Conversion::Send_Data conv[4];
Conversion::Available_Data C_Data[4];

// CAN ピン設定（STM32F446RE 用）
CAN can(PA_11, PA_12); // 1 Mbps

Ticker cansend;
CANMessage canMsgSend;
CANMessage canMsgReceive;

void send_current() {
    conv[0].update(C_Data[0].Become(5));
    canMsgSend.data[0] = conv[0].High_Byte;
    canMsgSend.data[1] = conv[0].Low_Byte;
    canMsgSend.data[2] = 0;
    canMsgSend.data[3] = 0;
    canMsgSend.data[4] = 0;
    canMsgSend.data[5] = 0;
    canMsgSend.data[6] = 0;
    canMsgSend.data[7] = 0;
}

void send() {
    can.write(canMsgSend);
}

int main() {
    can.frequency(1000000);           // DJI ESCは1Mbps
    can.mode(CAN::Normal);
    canMsgSend.id = 0x200;            // DJI ESCの制御CAN ID（0x200）
    canMsgSend.len = 8;               // データ長8バイト（4台分）
    for (int i = 0; i < 8; i++) canMsgSend.data[i] = 0;
    cansend.attach(&send_current, 10ms);
    
    while (true) {
        // printf("Sending current to C620 ESC...\n");
        send();
        if (can.read(canMsgReceive)) {
            // printf("1:  %d\n", canMsgReceive.data[0]);
            // printf("2:  %d\n", canMsgReceive.data[1]);
            int comes = canMsgReceive.data[0] << 8 | canMsgReceive.data[1];
            printf("%d\n", comes);
        }
        ThisThread::sleep_for(10ms); // 送信周期：10ms（100Hz くらいが理想）
    }
}