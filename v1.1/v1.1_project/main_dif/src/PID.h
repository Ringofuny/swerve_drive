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
        }
        double PID_move(double goal, double current);
        double PD_move(double goal, double current);
        double P_move(double goal, double current);

    private:
        double kp;
        double ki;
        double kd;
        double goal;
        double current;
        double sum;
        double dif;
        double error_pre;
        double error;
        double out;
};

#endif
