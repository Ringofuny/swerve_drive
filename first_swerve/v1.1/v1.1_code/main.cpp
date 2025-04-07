#include "include_lib.h"
#include "SerialCtrl_OS6/SerialCtrl.h"
using namespace NITSC;
SerialCtrl fep(uart);

calclation_main::Calclation calc_main;
calclation_sub::Calclation calc_sub;

int main(){
    /*-----------------------------変数の宣言------------------------------*/
    float X = 0;
    float Y = 0;
    send.FAIL = 0;
    send.mode = 0;
    bool loop = true; // ループ設定用のフラグ
    int fail = 0; //ループ内のカウンターの初期化

    preparation();

    while (loop){
        if (fep.tryReceive()) {
            fail = 0;
            led1 = 1;
            if (!Mode) {
                initial_set();
            } else {
                calc_main.rad = ((2 * M_PI / 8192.0) * -enc[0].enc_count) - M_PI;

                setNeutral();
                inputCAN_SticData();

                X = (R[0]);
                Y = (R[1]);

                calc_sub.my_output(calc_sub.g_speed, calc_sub.c_speed);

                if (L != 0) {
                    //足回り回す
                    dig2 = L > 0 ? 1 : 0; // Lが0より大きいなら0を返しそうでなければ1を返す
                    pwm2 = ((L > 0 ? 1 : 0));
                    dig3 = (L < 0) ? 1 : 0; 
                    pwm3 = (L < 0) ? 1-calc_sub.calclated_PWM() : calc_sub.calclated_PWM();
                } else {            
                    if ((Y == 0 && X == 0)) {
                        dig2 = 0;
                        dig3 = 0;
                        pwm2 = 1;
                        pwm3 = 1;
                    } else {
                        calc_main.G = (atan2f(Y, X));  
                        calc_main.my_output(calc_main.G, calc_main.rad);
                        calc_main.digital = calc_main.my_digital(calc_main.G, calc_main.rad);

                        if (calc_main.digital == 0) {
                            dig2 = 0;
                            dig3 = 0;
                            pwm2 = 1-calc_main.calclated_PWM();
                            pwm3 = calc_sub.calclated_PWM();    
                        } else if (calc_main.digital == 2) {
                            dig2 = 0;
                            dig3 = 0;
                            pwm2 = 1;
                            pwm3 = 1;
                        }  else if (calc_main.digital == 1) {
                            dig2 = 1;
                            dig3 = 1;
                            pwm2 = calc_main.calclated_PWM();
                            pwm3 = 1-calc_sub.calclated_PWM();
                        }
                    }
                }
            }
        } else {
            fail++;
            //カウンターが5を超えたら通信失敗と表示
            if (fail > 10) {
                emergency_stop();
                printf("connection false\n");
            }
        }
    }
}

// /*------------------------------角速度取得 → 合わせに行く------------------------------*/
void speed() { // 2->3合わせる
    calc_sub.g_speed = ((2 * M_PI) * enc[1].enc_count) / (8192.0 * 0.01);
    calc_sub.c_speed = ((2 * M_PI) * enc[2].enc_count) / (8192.0 * 0.01);
    enc[1].enc_count = 0;
    enc[2].enc_count = 0;
}

/*------------------------------モーターの周期の設定------------------------------*/
void setPwm_period() {
    pwm1.period_us(83); //モータの周期の設定
    pwm2.period_us(83); //モータの周期の設定
    pwm3.period_us(83); //モータの周期の設定
    pwm4.period_us(83); //モータの周期の設定
    pwm5.period_us(83); //モータの周期の設定
    pwm6.period_us(83); //モータの周期の設定
}

/*------------------------------立ち上がり立ち下がりの設定------------------------------*/
void rise_fall() {
    swi1_a.rise(callback(counter1_a_rise));
    swi1_a.fall(callback(counter1_a_fall));
    swi1_b.rise(callback(counter1_b_rise));
    swi1_b.fall(callback(counter1_b_fall));

    swi2_a.rise(callback(counter2_a_rise));
    swi2_a.fall(callback(counter2_a_fall));
    swi2_b.rise(callback(counter2_b_rise));
    swi2_b.fall(callback(counter2_b_fall));

    swi3_a.rise(callback(counter3_a_rise));
    swi3_a.fall(callback(counter3_a_fall));
    swi3_b.rise(callback(counter3_b_rise));
    swi3_b.fall(callback(counter3_b_fall));
}

/*------------------------------CANメッセージの送信部分------------------------------*/
void sendValues() {
    // 送信するデータを設定
    canMsgSend.data[0] = send.RX;
    canMsgSend.data[1] = send.RY;
    canMsgSend.data[2] = send.LY;
    canMsgSend.data[3] = send.mode;
    canMsgSend.data[4] = send.comp;
    canMsgSend.data[5] = send.goback;
    canMsgSend.data[6] = send.FAIL;

    // メッセージの送信確認用
    if (can.write(canMsgSend)) {
        led2 = 1;
    } else {
        led2 = 0;
    }
    // メッセージ送信            
}

/*------------------------------最初の角度をセットする関数------------------------------*/
void initial_set() {
    currentButtonStat[0] = fep.getData(DualShock4::BUTTON_0) & (DualShock4::UP);  // ボタンの状態を取得
    currentButtonStat[1] = fep.getData(DualShock4::BUTTON_0) & (DualShock4::RIGHT);  // ボタンの状態を取得
    currentButtonStat[2] = fep.getData(DualShock4::BUTTON_0) & (DualShock4::DOWN);  // ボタンの状態を取得
    currentButtonStat[3] = fep.getData(DualShock4::BUTTON_0) & (DualShock4::LEFT);  // ボタンの状態を取得
    if (send.mode == 0) {
        if (fep.getData(DualShock4::BUTTON_0) & (DualShock4::UP)) {
            send.goback = 0;
            dig2 = 1;
            dig3 = 1;
            pwm2 = 0.5;
            pwm3 = 0.5;
        } else if (fep.getData(DualShock4::BUTTON_0) & (DualShock4::DOWN)) {
            send.goback = 1;
            dig2 = 0;
            dig3 = 0;
            pwm2 = 0.5;
            pwm3 = 0.5;
        } else {
            send.goback = 2;
            dig2 = 0;
            dig3 = 0;
            pwm2 = 1;
            pwm3 = 1;
        }
        if (fep.getData(DualShock4::BUTTON_0) & (DualShock4::LEFT) && !prebtn[3]) {
            send.mode += 1;
            if (send.mode > 3) send.mode = 0;
            if (send.mode > 4) {
                send.mode = 0;
            }
        } else if (fep.getData(DualShock4::BUTTON_0) & (DualShock4::RIGHT) && !prebtn[1]) {
            send.mode -= 1;
            if (send.mode <= 0) send.mode = 0;
            if (send.mode > 4) {
                send.mode = 3;
            }
        }
        
    } else {
            if (fep.getData(DualShock4::BUTTON_0) & (DualShock4::UP)) {
            send.goback = 0;
        } else if (fep.getData(DualShock4::BUTTON_0) & (DualShock4::DOWN)) {
            send.goback = 1;
        } else {
            send.goback = 2;
        }
        if (fep.getData(DualShock4::BUTTON_0) & (DualShock4::LEFT) && !prebtn[3]) {
            send.mode += 1;
            if (send.mode > 3) send.mode = 0;
            if (send.mode > 4) {
                send.mode = 0;
            }
        } else if (fep.getData(DualShock4::BUTTON_0) & (DualShock4::RIGHT) && !prebtn[1]) {
            send.mode -= 1;
            if (send.mode <= 0) send.mode = 0;
            if (send.mode > 3) {
                send.mode = 3;
            }
        }
        dig2 = 0;
        dig3 = 0;
        pwm2 = 1;
        pwm3 = 1;
    }
    if (fep.getData(DualShock4::BUTTON_1) & (DualShock4::PS)) {
        check++;
        if (check >= 2) {
            enc[0].enc_count = 2048;
            send.comp = 1;
            Mode = true;
        }
    }
    prebtn[0] = currentButtonStat[0];  // ボタンの状態を取得
    prebtn[1] = currentButtonStat[1];  // ボタンの状態を取得
    prebtn[2] = currentButtonStat[2];  // ボタンの状態を取得
    prebtn[3] = currentButtonStat[3];  // ボタンの状態を取得
    send.RX = fep.getData(DualShock4::RX);
    send.RY = fep.getData(DualShock4::RY);
    send.LY = fep.getData(DualShock4::LY);
    can.write(canMsgSend);
    ThisThread::sleep_for(1ms);
}
/*
最初の角度をセットする関数に関してはもう少し練りたい
*/

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

/*------------------------------canの設定（初期化含む）------------------------------*/
void CAN_Setting() {
    can.frequency(1000000);
    can.mode(CAN::Normal);
    ticker.attach(&sendValues,700us);
    speed_tim.attach(&speed, 1ms);
    canMsgSend.id = 0x01;  // ID設定
    canMsgSend.len = 7;     // 3バイトデータ
    for (int i = 0; i < 7; i++) {
        canMsgSend.data[i] = 0;
    }
}

/*------------------------------スティックのデータを送る（CAN）------------------------------*/
void inputCAN_SticData() {
    send.RX = fep.getData(DualShock4::RX);
    send.RY = fep.getData(DualShock4::RY);
    send.LY = fep.getData(DualShock4::LY);
    can.write(canMsgSend);
    ThisThread::sleep_for(1ms);
}

void preparation() {
    rise_fall(); // エンコーダーの立ち上がり立ち下がりの設定
    setPwm_period(); // pwmの周期の設定
    CAN_Setting();
}

void emergency_stop() {
    dig1 = 0;
    dig2 = 0;
    dig3 = 0;
    dig4 = 0;
    dig5 = 0;
    dig6 = 0;
    pwm1 = 1;
    pwm2 = 1;
    pwm3 = 1;
    pwm4 = 1;
    pwm5 = 1;
    pwm6 = 1;
    led1 = 0;
}