#include "my_library.h"
#include "conversion.h"
Conversion::Send_Data conv[4];
Conversion::Available_Data C_Data[4];

Ctrl Controler;

int goal = 0;
int current = 0;

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

int main() {
    can.frequency(1000000);           // DJI ESCは1Mbps
    can.mode(CAN::Normal);
    canMsgSend.id = 0x200;            // DJI ESCの制御CAN ID（0x200）
    canMsgSend.len = 8;               // データ長8バイト（4台分）
    for (int i = 0; i < 8; i++) canMsgSend.data[i] = 0;
    canSend.attach(&send_current, 10ms);

    PID_in.attach(&Angle_Speed, 1ms); // 1msごとに制御送信


    while (1) {
        if (fep.tryReceive()) {
            settingCtrl();
            send();
            Steer_move.SetData(Controler.Data.R[0], Controler.Data.R[1], Controler.Data.L);
            if (can.read(canMsgReceive)) {
                switch (canMsgReceive.id) 
                {
                case 0x201:
                    current = (canMsgReceive.data[2] << 8 | canMsgReceive.data[3]);
                    break;
                
                case 0x0202:
                    goal = (canMsgReceive.data[2] << 8 | canMsgReceive.data[3]);
                    break;
                }
            }     
            printf("%f", output_current);         
            ThisThread::sleep_for(1ms);
        }
    }
}

void Angle_Speed() {
    Steer_move.goal_speed = goal;
    output_current = Steer_move.update(current);
}

/*------------------------------ニュートラルの設定------------------------------*/
void settingCtrl() {
    //X
    Controler.Data.R[0] = ctrl[0].setNeutral(fep.getData(DualShock4::RX));
    //Y 
    Controler.Data.R[1] = ctrl[1].setNeutral(fep.getData(DualShock4::RY));
    //L
    Controler.Data.L = ctrl[2].setNeutral(fep.getData(DualShock4::LY));
}