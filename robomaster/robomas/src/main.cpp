#include "mbed.h"
#include "rtos.h"

// CAN ピン設定（STM32F446RE 用）
CAN can(PA_11, PA_12); // 1 Mbps

Ticker cansend;
CANMessage canMsgSend;
CANMessage canMsgReceive;

void send_current(int16_t motor1, int16_t motor2 = 0, int16_t motor3 = 0, int16_t motor4 = 0) {
    canMsgSend.data[0] = motor1 >> 8;
    canMsgSend.data[1] = motor1 & 0xFF;
    canMsgSend.data[2] = motor2 >> 8;
    canMsgSend.data[3] = motor2 & 0xFF;
    canMsgSend.data[4] = motor3 >> 8;
    canMsgSend.data[5] = motor3 & 0xFF;
    canMsgSend.data[6] = motor4 >> 8;
    canMsgSend.data[7] = motor4 & 0xFF;
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
    cansend.attach(&send, 1ms);
    
    while (true) {
        // printf("Sending current to C620 ESC...\n");
        send_current(5000); // 適度に大きめ（安全範囲で）

        ThisThread::sleep_for(1ms); // 送信周期：10ms（100Hz くらいが理想）
    }
}
