#ifndef __encoder_h
#define __encoder_h

#include <Arduino.h>
#include <inttypes.h>

class Encoder
{
public:
    Encoder();
    inline int16_t read() const { return m_count; }
private:
    friend void int2_isr(void);
    friend void int3_isr(void);

    static volatile int16_t m_count;
};

#endif
