#include "steer.h" 
PID PD[2];

enum {target, current};

Ctrl my_wheels[2] = {
    {0, 0},
    {0, 0}
};

Steer::Steer() {
}

void Steer::SetData(float stX, float stY, float stY_L) {
    X[0] = stX;
    Y[0] = stY;
    L = stY_L;
}

float Steer::update(float CAN_Data_Count) { // rpm
    // 目標の角度
    X[1] = X[0];
    Y[1] = Y[0];
    my_wheels[target].angle = atan2f(Y[1], X[1]); // 
    
    // 現在の角度
    my_wheels[current].angle = ((2 * M_PI) * (CAN_Data_Count / 60.0)) * (0.01); // 

    // 速度
    my_wheels[target].speed = goal_speed; // 10ms 考える（多分かくどPDしてそこから速度の目標値出す）
    my_wheels[current].speed = CAN_Data_Count; // 10ms

    return PD[0].PD_speed_angle(my_wheels[target].angle,  my_wheels[current].angle, my_wheels[target].speed, my_wheels[current].speed);
}   
