#include "mbed.h"
#include "rtos.h"

CAN can(PA_11, PA_12); // CANピンの設定

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
void sendV();
int16_t fmap(double x, double in_min, double in_max, int16_t out_min, int16_t out_max);

Ticker canSend;
CANMessage canMsgSend;
CANMessage canMsgReceive;

int main() {
    can.frequency(1000000);
    can.mode(CAN::Normal);
    canSend.attach(&sendV,1ms);
    canMsgSend.id = 0x200;  // ID設定
    for (int i = 0; i < 2; i++) {
        canMsgSend.data[i] = 0;
    }

    for(;;) {
        sendV();
        can.write(canMsgSend);
        ThisThread::sleep_for(1ms);
        
        if (can.read(canMsgReceive)) {
            rxId = canMsgReceive.id;
            int16_t angle = canMsgReceive.data[0] << 8 | canMsgReceive.data[1];
            int16_t rpm = canMsgReceive.data[2] << 8 | canMsgReceive.data[3];
            int16_t amp = canMsgReceive.data[4] >> 8 | canMsgReceive.data[5];
            int8_t temp = canMsgReceive.data[6];

            printf("angle : %d, rpm : %d, amp : %d, temp : %d", angle, rpm, amp, temp);
        }    
    }
}

void sendV() {
    double motor_output_current_A = 0.5;
    int16_t motor_ouptut_byte = fmap(motor_output_current_A, 0, 20, 0, 16384);
    canMsgSend.data[0] = (motor_ouptut_byte >> 8) & 0xFF;
    canMsgSend.data[1] = (motor_ouptut_byte) & 0xFF;
}

int16_t fmap(double x, double in_min, double in_max, int16_t out_min, int16_t out_max) {
    return (x - in_min) * ((double)(out_max - out_min)) / (in_max - in_min) + out_min;
}