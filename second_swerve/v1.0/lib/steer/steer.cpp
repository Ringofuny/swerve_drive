#include "steer.h" 
PID PD[2];

enum {target, current};

Ctrl my_wheels[2] = {
    // 初期化
    {0, 0, 0},
    {0, 0, 0}
};


Steer::Steer() {
    goal_speed=0.0;
}

void Steer::SetData(float stX, float stY, float stY_L) {
    // こっちの関数に値を渡す
    X[0] = stX;
    Y[0] = stY;
    L = stY_L;
}

float Steer::update(int CAN_Data_Count) { // rpm
    /*-- 一番重要でPID制御の値を返す（角度）
            角速度を渡して（ロボマスの２番目のデータ 配列でいうと[2], [3]番）floatで
            処理済みのデータを返す */
    // 目標の角度
    X[1] = X[0];
    Y[1] = Y[0];
    my_wheels[target].angle = atan2f(Y[1], X[1]); // 
    
    // 現在の角度
    my_wheels[current].angle += ((2 * M_PI) * (CAN_Data_Count / 60.0)) * (0.01); // 
    my_wheels[current].rad = normalize_angle(my_wheels[current].angle);
    
    return PD[0].PID_angle(my_wheels[target].angle, my_wheels[current].rad);
}

float Steer::speed(int rpm , int goal) {
    // 速度を制御する予定
    my_wheels[target].speed = (goal / 60.0f) * 0.001f;
    my_wheels[current].speed = (rpm / 60.0f) * 0.001f;
    // 速度
    return PD[1].PID_move(my_wheels[target].speed, my_wheels[current].speed);
}

float Steer::normalize_angle(float angle_rad) {
    // ラジアンを出す時に使う
    while (angle_rad > M_PI) angle_rad -= 2 * M_PI;
    while (angle_rad < -M_PI) angle_rad += 2 * M_PI;
    return angle_rad;
}
