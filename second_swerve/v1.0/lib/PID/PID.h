#ifndef PID_H
#define PID_H
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <stdio.h>
#include <math.h>

class PID {
    public:  
        PID() {
            kp = 0.0040000225f;   // 応答の速さ
            ki = 0.000001f;   // 積分項は最初オフ
            kd = 0.000001f;   // 減速のための微分項
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
        float PD_angle(int goal, int current);
        float P_move(float goal, float current);
        float PD_speed_angle(float g_angle, float c_angle, float g_speed, float c_speed);
        float normalize_angle(float angle_rad);
        float PID_angle(float goal, float current);
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
