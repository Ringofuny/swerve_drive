#include "PID.h"

float PID::PID_move(float goal, float current)
{
    error = goal - current;
    // 8192をまたぐ場合の補正（循環）
    if (error > 4096) error -= 8192;
    if (error < -4096) error += 8192;
    sum += error;
    dif = error - error_pre;

    out = (error * kp) + (sum * ki) + (dif * kd);

    error = error_pre;

    return out;
}

float PID::PD_move(float goal, float current)
{
    error = goal - current;
    if (error > 4096) error -= 8192;
    if (error < -4096) error += 8192;
    dif = error - error_pre;

    out = (error * kp) + (dif * kd); 
    
    error = error_pre;

    return out;
}

float PID::P_move(float goal, float current) {
    error = goal - current;
    if (error > 4096) error -= 8192;
    if (error < -4096) error += 8192;

    out = (error * kp);

    return out;
}

float PID::PD_speed_angle(float g_angle, float c_angle, float g_speed, float c_speed)
{
    angle_error = g_angle - c_angle;
    speed_error = g_speed - c_speed;

    return ((angle_error * kp) + (speed_error * kd));
}
