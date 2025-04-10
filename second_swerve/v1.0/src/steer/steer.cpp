#include "steer.h" 
PID PD;

wheel my_wheels[2] = {
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

float Steer::update(float CAN_Data_Count) {
    // 目標の角度
    X[1] = X[0];
    Y[1] = Y[0];
    my_wheels[0].angle = atan2f(Y[1], X[1]); // 0 -> target

    // 現在の角度
    my_wheels[1].angle = ((CAN_Data_Count / 8192) * (2 * M_PI)) - M_PI; // 1 -> current

    // 速度
    my_wheels[0].speed = ((2 * M_PI) * CAN_Data_Count) / (8192.0 * 0.01); // 0 -> target
    my_wheels[1].speed = ((2 * M_PI) * CAN_Data_Count) / (8192.0 * 0.01); // 1 -> current

    return PD.PD_speed_angle( my_wheels[0].angle,  my_wheels[1].angle, my_wheels[0].speed, my_wheels[1].speed);
}   
