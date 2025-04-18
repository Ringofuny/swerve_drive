#include "include_lib.h"
#include "SerialCtrl_OS6/SerialCtrl.h"
using namespace NITSC;

PID PD[2];

CAN can(PA_11, PA_12); // CANピンの設定

/*-----------------------------角速度調整用------------------------------*/
Ticker speed_tim; //* 角速度調整を周期的やるためのタイマーの宣言
void speed();
/*-----------------------------角速度調整用------------------------------*/

/*------------------------------CAN送信の設定------------------------------*/
CANMessage canMsgSend;
Ticker ticker; // canを周期的やるためのタイマーの宣言
void sendValues();
/*------------------------------CAN送信の設定------------------------------*/

UnbufferedSerial uart(PC_10, PC_11, 38400);
SerialCtrl fep(uart);

int main(){
    rise_fall(); // エンコーダーの立ち上がり立ち下がりの設定
    setPwm_period(); // pwmの周期の設定

    /*------------------------------canの設定（初期化含む）------------------------------*/
    can.frequency(1000000);
    can.mode(CAN::Normal);
    ticker.attach(&sendValues,700us);
    speed_tim.attach(&speed, 1ms);
    canMsgSend.id = 0x01;  // ID設定
    canMsgSend.len = 7;     // 3バイトデータ
    for (int i = 0; i < 7; i++) {
        canMsgSend.data[i] = 0;
    }
    /*------------------------------canの設定（初期化含む）------------------------------*/

    /*-----------------------------変数の宣言------------------------------*/
    float R[2];
    float L;
    double goal[2] = {0, 0};
    float output[2] = {0, 0};
    double real_pwm[2] = {0, 0};
    int digital[2] = {2, 2};
    float X = 0;
    float Y = 0;
    float G = 0;
    float rad = 0;
    send.FAIL = 0;
    send.mode = 0;
    
    bool loop = true; // ループ設定用のフラグ
    int fail = 0; //ループ内のカウンターの初期化
    /*-----------------------------変数の宣言------------------------------*/

    while (loop){
        if (fep.tryReceive()) {
            fail = 0;
            led1 = 1;
            if (!Mode) {
                initial_set();
            } else {
                rad = ((2 * M_PI / 8192.0) * -enc_count) - M_PI;
                
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

                send.RX = fep.getData(DualShock4::RX);
                send.RY = fep.getData(DualShock4::RY);
                send.LY = fep.getData(DualShock4::LY);
                can.write(canMsgSend);
                ThisThread::sleep_for(1ms);

                X = (R[0]);
                Y = (R[1]);

                // 角速度(speed)を合わせる計算モーター２
                output[1] = PD[1].PID_move(g_speed, c_speed);
                goal[1] = MAXIN::fmin(MAXIN::fmax(output[1], 0.0), 1.0);
                real_pwm[1] = -goal[1];
                

                if (L != 0) {
                    //足回り回す
                    dig2 = L > 0 ? 1 : 0; // Lが0より大きいなら0を返しそうでなければ1を返す
                    pwm2 = ((L > 0 ? 1 : 0));
                    dig3 = (L < 0) ? 1 : 0; 
                    pwm3 = (L < 0) ? 1-real_pwm[1] : real_pwm[1];
                } else {            
                    if ((Y == 0 && X == 0)) {
                        dig2 = 0;
                        dig3 = 0;
                        pwm2 = 1;
                        pwm3 = 1;
                    } else {
                        G = (atan2f(Y, X));  

                        // speedの元になる計算モーター１
                        output[0] = PD[0].PD_move(G, rad); // そのままarctanを目標値に
                        digital[0] = digitalWrite_f(G, rad);
                        goal[0] = MAXIN::fmin((output[0] / 1.58), 1.0);
                        real_pwm[0] = MAXIN::fmin(fabs(goal[0]), 1.0);

                        printf("%d\n", digital[0]);

                        if (digital[0] == 0) {
                            dig2 = 0;
                            dig3 = 0;
                            pwm2 = 1-real_pwm[0];
                            pwm3 = real_pwm[1];    
                        } else if (digital[0] == 2) {
                            dig2 = 0;
                            dig3 = 0;
                            pwm2 = 1;
                            pwm3 = 1;
                        }  else if (digital[0] == 1) {
                            dig2 = 1;
                            dig3 = 1;
                            pwm2 = real_pwm[0];
                            pwm3 = 1-real_pwm[1];
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

/*------------------------------エンコーダーの関数１------------------------------*/
void counter1_a_rise(){
    if(enc1_b == 1){
        enc_count--;
    }else{
        enc_count++;
    }
}

void counter1_a_fall(){
    if(enc1_b == 1){
        enc_count++;
    }else{
        enc_count--;
    }
}

void counter1_b_rise(){
    if(enc1_a == 1){
        enc_count++;
    }else{
        enc_count--;
    }
}

void counter1_b_fall(){
    if(enc1_a == 1){
        enc_count--;
    }else{
        enc_count++;
    }
}
/*------------------------------エンコーダーの関数１------------------------------*/

/*------------------------------エンコーダーの関数２------------------------------*/
void counter2_a_rise(){
    if(enc2_b == 1){
        enc2_count--;
    }else{
        enc2_count++;
    }
}

void counter2_a_fall(){
    if(enc2_b == 1){
        enc2_count++;
    }else{
        enc2_count--;
    }
}

void counter2_b_rise(){
    if(enc2_a == 1){
        enc2_count++;
    }else{
        enc2_count--;
    }
}

void counter2_b_fall(){
    if(enc2_a == 1){
        enc2_count--;
    }else{
        enc2_count++;
    }
}
/*------------------------------エンコーダーの関数２------------------------------*/

/*------------------------------エンコーダーの関数３------------------------------*/
void counter3_a_rise(){
    if(enc3_b == 1){
        enc3_count--;
    }else{
        enc3_count++;
    }
}

void counter3_a_fall(){
    if(enc3_b == 1){
        enc3_count++;
    }else{
        enc3_count--;
    }
}

void counter3_b_rise(){
    if(enc3_a == 1){
        enc3_count++;
    }else{
        enc3_count--;
    }
}

void counter3_b_fall(){
    if(enc3_a == 1){
        enc3_count--;
    }else{
        enc3_count++;
    }
}
/*------------------------------エンコーダーの関数３------------------------------*/

/*------------------------------角速度取得 → 合わせに行く------------------------------*/
void speed() { // 2->3合わせる
    g_speed = ((2 * M_PI) * enc2_count) / (8192.0 * 0.01);
    c_speed = ((2 * M_PI) * enc3_count) / (8192.0 * 0.01);
    enc2_count = 0;
    enc3_count = 0;
}
/*------------------------------角速度取得 → 合わせに行く------------------------------*/

/*------------------------------モーターの周期の設定------------------------------*/
void setPwm_period() {
    pwm1.period_us(83); //モータの周期の設定
    pwm2.period_us(83); //モータの周期の設定
    pwm3.period_us(83); //モータの周期の設定
    pwm4.period_us(83); //モータの周期の設定
    pwm5.period_us(83); //モータの周期の設定
    pwm6.period_us(83); //モータの周期の設定
}
/*------------------------------モーターの周期の設定------------------------------*/

/*------------------------------立ち上がり立ち下がりの設定------------------------------*/
void rise_fall() {
    swi1_a.rise(counter1_a_rise);
    swi1_a.fall(counter1_a_fall);
    swi1_b.rise(counter1_b_rise);
    swi1_b.fall(counter1_b_fall);

    swi2_a.rise(counter2_a_rise);
    swi2_a.fall(counter2_a_fall);
    swi2_b.rise(counter2_b_rise);
    swi2_b.fall(counter2_b_fall);

    swi3_a.rise(counter3_a_rise);
    swi3_a.fall(counter3_a_fall);
    swi3_b.rise(counter3_b_rise);
    swi3_b.fall(counter3_b_fall);
}
/*------------------------------立ち上がり立ち下がりの設定------------------------------*/

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
/*------------------------------CANメッセージの送信部分------------------------------*/

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
            enc_count = 2048;
            send.comp = 1;
            Mode = true;
        }
    }
    prebtn[0] = currentButtonStat[0];  // ボタンの状態を取得
    prebtn[1] = currentButtonStat[1];  // ボタンの状態を取得
    prebtn[2] = currentButtonStat[2];  // ボタンの状態を取得
    prebtn[3] = currentButtonStat[3];  // ボタンの状態を取得
    // printf("%d, %d, %d\n", send.mode, send.comp, send.goback);
    send.RX = fep.getData(DualShock4::RX);
    send.RY = fep.getData(DualShock4::RY);
    send.LY = fep.getData(DualShock4::LY);
    can.write(canMsgSend);
    ThisThread::sleep_for(1ms);
}
/*------------------------------最初の角度をセットする関数------------------------------*/
/*
最初の角度をセットする関数に関してはもう少し練りたい
*/

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