#pragma once
#include "PID.h"
#include "func.h"
extern PID PD[2];

namespace calclation_main {
    class Calclation {    
        public:
            double real_pwm;
            int digital;
            float G;
            float rad;
            Calclation();
            void my_output(float G, float rad);
            int my_digital(float G, float rad);
            float go_mygoal();
            float calclated_PWM();
        private:
            double goal;
            float output;
    };
} // namespace calclation_main


namespace calclation_sub {
    class Calclation {    
        public:
            double real_pwm;
            int digital;
            float g_speed;
            float c_speed;
            Calclation();
            void my_output(float G, float rad);
            float go_mygoal();
            float calclated_PWM();
        private:
            double goal;
            float output;        
    };
} // namespace calclation_sub 