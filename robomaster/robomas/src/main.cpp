#include "mbed.h"
#include "rtos.h"

// CAN ピン設定（STM32F446RE 用）
CAN can(PA_11, PA_12, 1000000); // 1 Mbps

void send_current(int16_t motor1, int16_t motor2 = 0, int16_t motor3 = 0, int16_t motor4 = 0) {
    uint16_t motor_id = 0x200; // 指令 ID

    char DATA[8] = {0};
    DATA[0] = motor1 >> 8;
    DATA[1] = motor1 & 0xFF;
    DATA[2] = motor2 >> 8;
    DATA[3] = motor2 & 0xFF;
    DATA[4] = motor3 >> 8;
    DATA[5] = motor3 & 0xFF;
    DATA[6] = motor4 >> 8;
    DATA[7] = motor4 & 0xFF;

    CANMessage msg(motor_id, DATA, 8);
    can.write(msg);
}


int main() {
    printf("Sending current to C620 ESC...\n");

    while (true) {
        send_current(5000); // 適度に大きめ（安全範囲で）

        ThisThread::sleep_for(10ms); // 送信周期：10ms（100Hz くらいが理想）
    }
}
