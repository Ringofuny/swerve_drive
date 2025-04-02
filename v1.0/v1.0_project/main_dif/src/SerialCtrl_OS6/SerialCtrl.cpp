#include "SerialCtrl.h"
#include <chrono>


namespace NITSC
{

    SerialCtrl::SerialCtrl(mbed::UnbufferedSerial& serial) : _serial(serial) {}


    uint8_t SerialCtrl::getByte() 
    {
        _timer.start();
        _timer.reset();

        while( !_serial.readable() ) 
        {
            auto elapse = chrono::duration_cast<chrono::milliseconds>( _timer.elapsed_time() );

            if(elapse > _timeout) {
                return 0x00;
            }
        }

        uint8_t byte;

        _serial.read(&byte, 1);

        return byte;
    }
    

    bool SerialCtrl::tryReceive()
    {
        int fail = 0;

        while(true) {
            if(getByte() == _header) {
                uint8_t sum = 0;

                for(int i=0; i<_DATA_LENGTH; ++i) 
                    _buffer[i] = getByte();
                
                for(int i=0; i<_DATA_LENGTH-1; ++i) 
                    sum += _buffer[i];


                if(sum == _buffer[_DATA_LENGTH-1] && getByte() == _footer) {
                    for(int i=0; i<_DATA_LENGTH-1; ++i) 
                        _data[i] = _buffer[i];

                    return true;
                } else {
                    fail++;
                }

            } else {
                fail++;
            }

            if(fail >= _retry) {
                return false;
            }
        }  // end of "while(true)"
    }


    uint8_t SerialCtrl::getData(int pos)
    {
        if(pos >= 0 && pos < _DATA_LENGTH) {
            return _data[pos];
        } else {
            return 0x00;
        }
    }


    // header
    void SerialCtrl::setHeader()
    {
        _header = _DEF_HEADER;
    }

    void SerialCtrl::setHeader(uint8_t header)
    {
        _header = header;
    }
    

    // footer
    void SerialCtrl::setFooter()
    {
        _footer = _DEF_FOOTER;
    }

    void SerialCtrl::setFooter(uint8_t footer)
    {
        _footer = footer;
    }


    // timeout
    void SerialCtrl::setTimeout()
    {
        _timeout = _DEF_TIMEOUT;
    }

    void SerialCtrl::setTimeout(chrono::milliseconds timeout)
    {
        _timeout = timeout;
    }

    
    // retry
    void SerialCtrl::setRetry()
    {
        _retry = _DEF_RETRY;
    }

    void SerialCtrl::setRetry(int retry)
    {
        _retry = retry;
    }
}