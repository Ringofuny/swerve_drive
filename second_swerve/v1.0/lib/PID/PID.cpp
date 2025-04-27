#include "PID.h"

template <typename T>
T clamp(T val, T low, T high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}

float PID::PID_move(float goal, float current)
{
    error = goal - current;
    
    sum += error;
    dif = error - error_pre;

    out = (error * kp) + (sum * ki) + (dif * kd);

    error_pre = error;

    return out;
}

float PID::PID_angle(float goal, float current) {
    float error = goal - current;
    error = normalize_angle(error);  // ← ここでラップ補正する！

    sum += error;
    dif = error - error_pre;

    out = (error * kp) + (sum * ki) + (dif * kd);
    error_pre = error;

    return out;
}

float PID::PD_move(float goal, float current)
{
    error = goal - current;
    dif = error - error_pre;

    out = (error * kp) + (dif * kd); 
    
    error_pre = error;

    return out;
}

float PID::PD_angle(int goal, int current) {
    error = normalize_angle(goal - current);
    dif = error - error_pre;

    out = (error * kp) + (dif * kd); 
    
    error_pre = error;

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

float PID::normalize_angle(float angle_rad) {
    while (angle_rad > M_PI) angle_rad -= 2 * M_PI;
    while (angle_rad < -M_PI) angle_rad += 2 * M_PI;
    return angle_rad;
}