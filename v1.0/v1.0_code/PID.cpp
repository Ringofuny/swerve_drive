#include "PID.h"

double PID::PID_move(double goal, double current)
{
    error = goal - current;
    sum += error;
    dif = error - error_pre;

    out = (error * kp) + (sum * ki) + (dif * kd);

    error = error_pre;

    return out;
}

double PID::PD_move(double goal, double current)
{
    error = goal - current;
    dif = error - error_pre;

    out = (error * kp) + (dif * kd); 
    
    error = error_pre;

    return out;
}

double PID::P_move(double goal, double current) {
    error = goal - current;

    out = (error * kp);

    return out;
}
