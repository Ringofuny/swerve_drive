#include "my_library.h"
#include "conversion.h"
Conversion::Send_Data conv[4];
Conversion::Available_Data C_Data[4];

Ticker my_Ctrl;
Ticker cansend;

Ctrl Controler;

int16_t goal = 0;
int16_t current = 0;

float kakunin = 0;

void send() {
    // 意味ないけどcan送信の関数（割り込もうと思ったんだけどうまくいかなかった）
    can.write(canMsgSend);
}

float constrain(float x, float min_val, float max_val) {
    if (x < min_val) return min_val;
    if (x > max_val) return max_val;
    return x;
}

void send_current() { 
    // それぞれにcanに送信用のデータを代入          
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
    // canの設定
    can.frequency(1000000);
    can.mode(CAN::Normal);
    canMsgSend.id = 0x200;            // DJI ESCの制御CAN ID（0x200）
    canMsgSend.len = 8;               // データ長8バイト（4台分）
    for (int i = 0; i < 8; i++) canMsgSend.data[i] = 0; // 初期化
    // PID_in.attach(&Angle_Speed, 10ms); // 1msごとに制御送信
    my_Ctrl.attach(&settingCtrl, 1ms); // ニュートラルの設定（1ms周期）
    cansend.attach(&send_current, 10ms); // can送信データの代入

    while (1) { // 無限ループ
        if (fep.tryReceive()) { // 通信成功時
            // 関数に加工済みのコントローラーのデータを送る
            Steer_move[0].SetData(Controler.Data.R[0], Controler.Data.R[1], Controler.Data.L);
            Steer_move[1].SetData(Controler.Data.R[0], Controler.Data.R[1], Controler.Data.L);
            // send_current();
            send(); // canの送信（コードのどこでやるべきなんだろう）
            if (can.read(canMsgReceive)) { // can受信時
                switch (canMsgReceive.id) { // can IDの識別(受信)
                    case 0x201: // ID が 0x201の時
                    /* 
                        goalは速度の目標値を表す（角度の時は関係ない）
                        goal = (int16_t) <- これがないと符号が無くなってバグる    
                    */
                    goal = (int16_t)((canMsgReceive.data[2] << 8 | canMsgReceive.data[3])); // データが半分に分けられて送られてくるので合体
                    break;
                    
                    case 0x202: // ID が 0x202の時
                    // currentは現在速度を表す（角度の時は関係ない）
                    current = (int16_t)((canMsgReceive.data[2] << 8 | canMsgReceive.data[3])); // データが半分に分けられて送られてくるので合体
                    break;
                }
                /*
                    *conv[0].update(C_Data[0].Become(Steer_move[0].update(goal))); 

                    convはconvertionの略
                    C_Dataはchange dataの略
                    *1 ---------------------------------------------------------------------------------------
                    conv[0].update()    -- データを送信できる形にする関数（半分に分ける）
                    ex) [ 1001 0010 1111 1100 ] の時 [ 1001 0010 ](上位バイト)と[ 1111 1100 ](下位バイト)に分けて
                        それぞれ上位バイトは[ High_Byte ] に 下位バイトは[ Low_Byte ] に代入する
                    
                    *2 ---------------------------------------------------------------------------------------
                    C_Data[0].Become()  -- データをcanで送って制御できる形にして返す（int16_t）
                                           -10(A) ~ 10(A) を -16384 ~ 16384 の間のデータに変換
                    
                    *3 ---------------------------------------------------------------------------------------
                    Steer_move[0].update() -- 一番重要でPID制御の値を返す（角度）
                                           角速度を渡して（ロボマスの２番目のデータ 配列でいうと[2], [3]番）floatで
                                           処理済みのデータを返す
                */
                // Steer_move[0].goal_speed = Controler.Data.L;
                conv[0].update(C_Data[0].Become(Controler.Data.L));  
                // Steer_move[1].goal_speed = (goal*1.0);
                float out = Steer_move[1].speed(current, goal);
                kakunin = constrain(out, -1, 1);
                conv[1].update(C_Data[1].Become(kakunin));
            } else {
                printf("fail_CAN\n");
            }

            printf("%5d, %5f, %f, %5d\n", (C_Data[0].Become(Controler.Data.L)), (kakunin), Controler.Data.L, goal);
            ThisThread::sleep_for(10ms); // 10ms待つ 
        } else {
            printf("No");
        }
    }
}

void Angle_Speed() {
    // output_current = Steer_move[0].update(goal);   
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
