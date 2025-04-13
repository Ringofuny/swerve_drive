#pragma once

namespace Control {
    class Speed {
        public:
            float g_speed;
            float c_speed;
            void ctrl_Speed(int g_count, int c_count);
    };
}