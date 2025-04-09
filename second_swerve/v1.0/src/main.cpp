#include "mbed.h"
#include "rtos.h"
#include "SerialCtrl_OS6/SerialCtrl.h"
using namespace NITSC;
#include "gear.h"
Gear tooth[2]; // 0→角度　1→速度
#include "steer.h"
Steer Steer_move;
#include "counter.h"
using namespace Exceeded;
Count enc;

CAN can(PA_11, PA_12); // CANピン設定（STM32F446REなど）

UnbufferedSerial uart(PC_10, PC_11, 38400);
SerialCtrl fep(uart);

Ticker canSend;
CANMessage canMsgSend;
CANMessage canMsgReceive;

int16_t fmap(double x, double in_min, double in_max, int16_t out_min, int16_t out_max);
float R[2];
float L;
void setNeutral();
void sendV();

float output_current = 0;
float g_angle = 0;
void Angle_Speed();
Ticker PID_in;

int main() {
    can.frequency(1000000);           // DJI ESCは1Mbps
    can.mode(CAN::Normal);

    canMsgSend.id = 0x200;            // DJI ESCの制御CAN ID（0x200）
    canMsgSend.len = 8;               // データ長8バイト（4台分）
    for (int i = 0; i < 8; i++) canMsgSend.data[i] = 0;

    canSend.attach(&sendV, 1ms);      // 1msごとに制御送信
    PID_in.attach(&Angle_Speed, 1ms);      // 0.7msごとに制御送信

    int pre_angle = 0;
    // int error = 0;

    while (1) {
        if (fep.tryReceive()) {
            setNeutral();
            Steer_move.SetData(R[0], R[1], L);
            if (can.read(canMsgReceive)) {
                int16_t angle = canMsgReceive.data[0] << 8 | canMsgReceive.data[1];
                g_angle = angle;
            }
            enc.Adjustment(pre_angle, g_angle);
            int16_t cur_val = fmap(output_current, -20, 20, -16384, 16384);
            int16_t rotate = -cur_val;
            canMsgSend.data[0] = cur_val >> 8;
            canMsgSend.data[1] = cur_val & 0xFF;
            canMsgSend.data[2] = rotate >> 8;
            canMsgSend.data[3] = rotate & 0xFF;
            for (int i = 4; i < 8; i++) canMsgSend.data[i] = 0;               
            ThisThread::sleep_for(1ms);
        }
    }
}

void Angle_Speed() {
    output_current = Steer_move.update(g_angle);
}

void sendV() {
    can.write(canMsgSend); 
}

int16_t fmap(double x, double in_min, double in_max, int16_t out_min, int16_t out_max) {
    // 安全にクランプも入れる
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;
    return (int16_t)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

/*------------------------------ニュートラルの設定------------------------------*/
void setNeutral() {
    //X
    if (fep.getData(DualShock4::RX) >= 122 && fep.getData(DualShock4::RX) <= 132) {
        R[0] = 0; // ニュートラルの設定
    } else {
        R[0] = (float(fep.getData(DualShock4::RX)) - 128) * (-1) / 128 ; // 回転速度の設定
    }

    //Y
    if (fep.getData(DualShock4::RY) >= 122 && fep.getData(DualShock4::RY) <= 132) { // ニュートラルの設定
        R[1] = 0;
    } else {
        R[1] = (float(fep.getData(DualShock4::RY)) - 128) * (-1) / 128; // 回転速度の設定
    }

    //L
    if (fep.getData(DualShock4::LY) >= 122 && fep.getData(DualShock4::LY) <= 132) {
        L = 0; // ニュートラルの設定
    } else {
        L = (float(fep.getData(DualShock4::LY)) - 128) * (-1) / 128 ; // 回転速度の設定
    }
}