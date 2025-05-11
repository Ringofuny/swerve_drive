#include "steer.h" 
PID PD[2];

enum {target, current};

Ctrl my_wheels[2] = {
    // 初期化
    {0, 0, 0},
    {0, 0, 0}
};

float constrainf(float x, float a, float b) {
    if (x < a) return a;
    else if (x > b) return b;
    else return x;
}


Steer::Steer() {
    goal_speed=0.0;
}

void Steer::SetData(float stX, float stY, float stY_L) {
    // こっちの関数に値を渡す
    X[0] = stX;
    Y[0] = stY;
    L = stY_L;
}

float Steer::update(int CAN_Data_Count, float angle) { // rpm
    /*-- 一番重要でPID制御の値を返す（角度）
            角速度を渡して（ロボマスの２番目のデータ 配列でいうと[2], [3]番）floatで
            処理済みのデータを返す */
    // 目標の角度
    my_wheels[target].angle = angle; // 

    // 現在の角度
    // my_wheels[current].angle += ((2 * M_PI) * (CAN_Data_Count / 60.0)); // 
    // my_wheels[current].rad = ((2 * M_PI / 8192.0) * -CAN_Data_Count) - M_PI;
    float rad = (2.0f * M_PI * CAN_Data_Count / 8192.0f);  // 0〜2π
    rad = fmodf(rad + M_PI, 2.0f * M_PI) - M_PI;         // -π〜π に正規化
    // my_wheels[current].rad = -angle;                         // 符号反転が必要ならここで

    
    return PD[0].PID_angle(my_wheels[target].angle, my_wheels[current].rad);
}

float Steer::speed(int rpm , int goal) {
    // 速度を制御する予定
    my_wheels[target].speed = (goal / 60.0f) * 0.001;
    my_wheels[current].speed = (rpm / 60.0f) * 0.001;
    // 速度
    return PD[1].PID_move(my_wheels[target].speed, my_wheels[current].speed);
}

float Steer::normalize_angle(float angle_rad) {
    // ラジアンを出す時に使う
    while (angle_rad > M_PI) angle_rad -= 2 * M_PI;
    while (angle_rad < -M_PI) angle_rad += 2 * M_PI;
    return angle_rad;
}
