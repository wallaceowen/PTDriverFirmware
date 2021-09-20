// display.cpp

#include <Arduino.h>

#include "display.h"

#include <U8g2lib.h>  // LCD driver library
#include <SPI.h>

#include "ports.h"

// #define ROTATE U8G2_R2
#define ROTATE U8G2_R0
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(ROTATE);

Display::Display() :
    m_state(DS_WaitForUpdate),
    m_updated(false),
    m_render_cb(0),
    m_render_param(0)
{
    u8g2.begin();
}

void Display::set_cb(RenderCallback cb, void *param)
{
    m_render_cb = cb;
    m_render_param = param;
}

void Display::loop()
{
    // Serial.print("disp state: ");
    // Serial.println((int)m_state);
    this->render();
}

void Display::render()
{
    switch(m_state)
    {
        case DS_Init:
            u8g2.firstPage();
            m_state = DS_While;
            break;

        case DS_While:
        {
            if (m_render_cb)
                m_render_cb(&u8g2, m_render_param);
            if (!u8g2.nextPage())
                m_state = DS_WaitForUpdate;
        }
        case DS_WaitForUpdate:
        {
			if (m_updated)
			{
				m_state = DS_Init;
				m_updated = false;
			}
			break;
        }
    }
}

