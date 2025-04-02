#include "mbed.h"
#include "rtos.h"

#include "DualShock4.h"

namespace NITSC 
{
    class SerialCtrl 
    {
        private:
            // default setting values
            static constexpr int _DATA_LENGTH = 9;
            static constexpr uint8_t _DEF_HEADER = 0xAF;
            static constexpr uint8_t _DEF_FOOTER = 0xED;
            static constexpr chrono::milliseconds _DEF_TIMEOUT = 15ms;
            static constexpr int _DEF_RETRY = _DATA_LENGTH * 2;
            
            // member
            mbed::UnbufferedSerial& _serial;
            mbed::Timer _timer;

            uint8_t _header = _DEF_HEADER;
            uint8_t _footer = _DEF_FOOTER;
            chrono::milliseconds _timeout = _DEF_TIMEOUT;
            int _retry = _DEF_RETRY;

            uint8_t _buffer[_DATA_LENGTH] = {};
            uint8_t _data[_DATA_LENGTH - 1] = {};

            uint8_t getByte();

        public:
            SerialCtrl(mbed::UnbufferedSerial& serial);

            bool tryReceive();

            uint8_t getData(int pos);

            void setHeader();
            void setHeader(uint8_t header);
            
            void setFooter();
            void setFooter(uint8_t footer);

            void setTimeout();
            void setTimeout(chrono::milliseconds timeout);

            void setRetry();
            void setRetry(int retry);
    };
}
