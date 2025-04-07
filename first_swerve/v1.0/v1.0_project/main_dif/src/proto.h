#pragma once

/*-----------------------------エンコーダーの立ち上がり立ち下がりを指定する関数------------------------------*/
void counter1_a_rise();
void counter1_a_fall();
void counter1_b_rise();
void counter1_b_fall();

void counter2_a_rise();
void counter2_a_fall();
void counter2_b_rise();
void counter2_b_fall();

void counter3_a_rise();
void counter3_a_fall();
void counter3_b_rise();
void counter3_b_fall();
/*-----------------------------エンコーダーの立ち上がり立ち下がりを指定する関数------------------------------*/

/*-----------------------------main関数でやる初期設定群------------------------------*/
void rise_fall(); //* エンコーダーの立ち上がり立ち下がりの設定
void setPwm_period(); //* pwmの周期の設定
/*-----------------------------main関数でやる初期設定群------------------------------*/

/*-----------------------------初期位置設定の塊------------------------------*/
bool currentButtonStat[4] = {false, false, false, false};
bool prebtn[4] = {false, false, false, false};
bool Mode = false;
int check = 0;
void initial_set();
/*-----------------------------初期位置設定の塊------------------------------*/

/*-----------------------------通信失敗時の全停止関数------------------------------*/
void emergency_stop();
/*-----------------------------通信失敗時の全停止関数------------------------------*/