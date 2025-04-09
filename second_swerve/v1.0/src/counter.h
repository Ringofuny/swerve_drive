#pragma once 

namespace Exceeded {
    class Count {
        public:
            int Adjustment(int pre, int current);
        private:
            int error;
    };
}