// encoder.cpp

#include "encoder.h"

volatile int16_t Encoder::m_count = 0;

void int2_isr(void)
{
    if (digitalRead(3))
        --Encoder::m_count;
    else
        ++Encoder::m_count;
}

#ifdef USE_BOTH_PHASES
void int3_isr(void)
{
    if (digitalRead(2))
        --Encoder::m_count;
    else
        ++Encoder::m_count;
}
#endif

Encoder::Encoder()
{
    attachInterrupt(digitalPinToInterrupt(2), int2_isr, FALLING);
#ifdef USE_BOTH_PHASES
    attachInterrupt(digitalPinToInterrupt(3), int3_isr, RISING);
#endif
}
