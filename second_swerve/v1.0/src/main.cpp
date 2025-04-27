#include "my_library.h"
#include "conversion.h"
Conversion::Send_Data conv[4];
Conversion::Available_Data C_Data[4];

Ticker my_Ctrl;
Ticker cansend;

Ctrl Controler;

int goal = 0;
int current = 0;

void send() {
    can.write(canMsgSend);
}

void send_current() {           
    canMsgSend.data[0] = conv[0].High_Byte;
    canMsgSend.data[1] = conv[0].Low_Byte;
    canMsgSend.data[2] = conv[1].High_Byte;;
    canMsgSend.data[3] = conv[1].Low_Byte;
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
    my_Ctrl.attach(&settingCtrl, 1ms);
    // PID_in.attach(&Angle_Speed, 10ms); // 1msごとに制御送信
    cansend.attach(&send_current, 10ms);

    while (1) {
        if (fep.tryReceive()) {
            Steer_move.SetData(Controler.Data.R[0], Controler.Data.R[1], Controler.Data.L);
            // send_current();
            send();
            if (can.read(canMsgReceive)) {
                switch (canMsgReceive.id) {
                    case 0x201:
                    goal = (canMsgReceive.data[2] << 8 | canMsgReceive.data[3]);
                    break;
                    
                    case 0x202:
                    current = (canMsgReceive.data[2] << 8 | canMsgReceive.data[3]);
                    break;
                }
                Steer_move.goal_speed = goal;
                conv[0].update(C_Data[0].Become(Steer_move.update(goal)));  
                conv[1].update(0);
            } else {
                printf("KO");
            }
            printf("%f, %d\n", Steer_move.update(goal), 0);         
            ThisThread::sleep_for(10ms);
        } else {
            printf("No");
        }
    }
}

void Angle_Speed() {
    // output_current = Steer_move.update(goal);
    
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
