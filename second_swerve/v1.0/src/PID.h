#ifndef PID_H
#define PID_H

#include <stdio.h>

class PID {
    public:  
        PID() {
            kp = 0.05;
            ki = 0.5;
            kd = 1.0;
            goal = 0.0;
            current = 0.0;
            sum = 0.0;
            dif = 0.0;
            error_pre = 0.0;
            error = 0.0;
            out = 0.0;
            angle_error = 0.0;
            speed_error = 0.0;
        }
        float PID_move(float goal, float current);
        float PD_move(float goal, float current);
        float P_move(float goal, float current);
        float PD_speed_angle(float g_angle, float c_angle, float g_speed, float c_speed);

    private:
        float kp;
        float ki;
        float kd;
        float goal;
        float current;
        float sum;
        float dif;
        float error_pre;
        float error;
        float out;
        float angle_error;
        float speed_error;
};

#endif
