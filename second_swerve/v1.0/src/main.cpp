#include "my_library.h"

int main() {
    can.frequency(1000000);           // DJI ESCは1Mbps
    can.mode(CAN::Normal);

    canMsgSend.id = 0x200;            // DJI ESCの制御CAN ID（0x200）
    canMsgSend.len = 8;               // データ長8バイト（4台分）
    for (int i = 0; i < 8; i++) canMsgSend.data[i] = 0;

    canSend.attach(&sendV, 1ms);      // 1msごとに制御送信
    PID_in.attach(&Angle_Speed, 1ms); // 1msごとに制御送信


    while (1) {
        if (fep.tryReceive()) {
            settingCtrl();
            Steer_move.SetData(R[0], R[1], L);
            if (can.read(canMsgReceive)) {
                int16_t angle = canMsgReceive.data[0] << 8 | canMsgReceive.data[1];
                g_angle = angle;
            }
            int16_t cur_val = fmap(output_current, -20, 20, -16384, 16384);
            int16_t rotate = -cur_val;
            canMsgSend.data[0] = cur_val >> 8;
            canMsgSend.data[1] = cur_val & 0xFF;
            canMsgSend.data[2] = rotate >> 8;
            canMsgSend.data[3] = rotate & 0xFF;
            for (int i = 4; i < 8; i++) canMsgSend.data[i] = 0;               
            pre_angle = g_angle;
            ThisThread::sleep_for(1ms);
        }
    }
}

void Angle_Speed() {
    output_current = Steer_move.update(enc.Adjustment(pre_angle, g_angle));
}

void sendV() {
    can.write(canMsgSend); 
}

/*------------------------------ニュートラルの設定------------------------------*/
void settingCtrl() {
    //X
    R[0] = ctrl[0].setNeutral(fep.getData(DualShock4::RX));
    //Y 
    R[1] = ctrl[1].setNeutral(fep.getData(DualShock4::RY));
    //L
    L = ctrl[2].setNeutral(fep.getData(DualShock4::LY));
}