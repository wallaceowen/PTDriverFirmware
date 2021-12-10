// buttons.cpp

#include <Arduino.h>
#include <string.h>

#include "ports.h"
#include "buttons.h"

Buttons::Buttons(ButtonsCB cb, void*arg) : m_cb(cb), m_cbarg(arg)
// Buttons::Buttons(void)
{
    memset(m_last, 0, sizeof(m_last));
    pinMode(BTN_1, INPUT);
    pinMode(BTN_2, INPUT);
}

void Buttons::loop(void)
{
    for (int b = 0; b < Btn_Num; ++b)
    {
        int p = pressed(b);
        if (p != m_last[b])
        {
            m_cb(b, p, m_cbarg);
            m_last[b] = p;
        }
    }
}

// BtnSettleTime=500
// Check the buttons and update their state
bool Buttons::pressed(int which)
{
    if (which == 0)
        return digitalRead(BTN_1)?true:false;
    else if (which == 1)
        return digitalRead(BTN_2)?true:false;
    else
        return 0;
}

