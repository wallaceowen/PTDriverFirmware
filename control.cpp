// Include Arduino env (Serial, String, etc)
#include <Arduino.h>

#include <math.h> /* for abs() */

// I2C lib
// #include <Wire.h>

#include "ports.h"

#include "debug.h"

// DAC lib
#include "dac.h"

// PT100 conditioner
#include <Adafruit_MAX31865.h>

// Temp
#include "temp.h"

// Control
#include "control.h"

#include "cfield.h"

#define TARGET_5
// #define TARGET_3_3

#ifdef TARGET_5
#define MAX_ALLOWED_VOLTAGE 5.0
#define SCALE (1.0)
#elif defined(TARGET_3_3)
#define MAX_ALLOWED_VOLTAGE 3.3
#define SCALE (MAX_ALLOWED_VOLTAGE/5.0)
#endif

// #define TEST_DAC
#ifdef TEST_DAC
#define SHOW_DAC
#else
#define SHOW_FAHR
#endif

static DAC dac;

// init class statics for Control class
float Control::temperature = 0.0;
float Control::adj_temp = 0.0;
float Control::offset = 0.0;
float Control::voltage = 0.0;

float Control::last_temp = 0.0;
// float Control::last_adj_temp = 0.0;
float Control::last_offset = 0.0;
// float Control::last_v = 0.0;

// Render ourselves onto the gui
void render_cb(U8G2 &disp, void *param)
{
    Control *c = reinterpret_cast<Control*>(param);
    c->render_fields(disp);
}

Control::Control(Display *display, Temp *temp, Encoder &encoder)
	: m_display(display), m_encoder(encoder), m_temp(temp)
{
    dac.begin(DAC_ADDR);
	m_display->set_cb(render_cb, 0);
}

// Read the encoder, deal with offset
float Control::read_offset(void)
{
    // Get the knob offset
    float offset = ((float) m_encoder.read())/10.0;
    return offset;
}

// Save current temp, offset to last temp, offset
void Control::update_last_params()
{
	last_temp = temperature;
	// last_adj_temp = adj_temp;
	last_offset = offset;
	// last_v = v;
}

// Update the params controlled and displayed
void Control::update_params(float t, float a_temp, float off, float v)
{
	temperature = t;
	adj_temp = a_temp;
    offset = off;
    voltage = v;
}

// Does the new temp or offset differ from last time?
bool Control::params_changed()
{
	return ((temperature != last_temp)
		|| (offset != last_offset));
}

// Display is 128x64
void Control::render_fields(U8G2 &disp) const
{
	disp.setDrawColor(1);
	disp.setFontMode(1);
	static bool updated = false;
	char buffer[32];

    disp.setFont(u8g_font_courR10);
	String tstr(temperature);
	sprintf(buffer, "%s\xb0", tstr.c_str());
    u8g2_uint_t width = disp.getStrWidth(buffer);
    int height = disp.getMaxCharHeight();
	render_field(disp, Field(128, 0, 0, AlignLeft), buffer);

    float fahr = temperature * 9 / 5 + 32.0;
	String fstr(fahr);
	sprintf(buffer, "(%s\xb0%c)", fstr.c_str(), 'F');
	render_field(disp, Field(128-(width+2), width+2, 0, AlignRight), buffer);

	String ostr(offset);
	sprintf(buffer, "OFFSET %s\xb0", ostr.c_str());
	render_field(disp, Field(128, 0, height, AlignLeft), buffer);

	String astr(adj_temp);
	sprintf(buffer, "OUTPUT %s\xb0", astr.c_str());
	render_field(disp, Field(128, 0, height*2, AlignLeft), buffer);

	String vstr(voltage);
	sprintf(buffer, "VOLTS %s", vstr.c_str());
	render_field(disp, Field(128, 0, height*3, AlignLeft), buffer);
}

float v_from_off(float offset)
{
    return fabs(offset);
}

void Control::loop()
{
#ifdef DEBUG_TIME
    int32_t now = 0;
#endif

    // Read the temp
#ifdef DEBUG_TIME
    now = millis();
#endif

    Serial.println("control 1");


#ifdef DEBUG_TIME
    tell_how_long("read temp", millis()-now);
#endif

    // Get the temp then add the temp offset to get adjusted temp
    float temp = m_temp->get_temp();
    float offset = read_offset();
    float adj_temp = temp + offset;

    /* Scale value for 12-bit DAC whose full scale output is 5V
     *
     * We must constrain the output voltage to 3.0V, so whatever
     * PT100 temp is 3.0V that's our max temp.
     *
     */
#ifdef DEBUG_TIME
    now = millis();
#endif
    Serial.println("control 2");
#ifdef TEST_DAC
    float v = fabs(offset);
#else
    float v = Temp::compute_voltage(adj_temp);
    Serial.println("control 3");

    // Scale to MAX_ALLOWED_VOLTAGE
    v *= SCALE;
#endif
    if (v >= MAX_ALLOWED_VOLTAGE)
        v = MAX_ALLOWED_VOLTAGE;
#ifdef DEBUG_TIME
    tell_how_long("compute voltage", millis()-now);
#endif

#ifdef DEBUG_TIME
    now = millis();
#endif
    update_params(temp, adj_temp, offset, v);
#ifdef DEBUG_TIME
    tell_how_long("update params", millis()-now);
#endif
    Serial.println("control 4");

#ifdef DEBUG_TIME
    now = millis();
#endif
    if (params_changed())
    {
        if (m_display->ready())
        {
            m_display->update();
            update_last_params();
        }
    }
    Serial.println("control 5");
#ifdef DEBUG_TIME
    tell_how_long("update params", millis()-now);
#endif

    /*
     * This is for the DAC scaling.
     * We need a value between 0 and 4095 for the
     * span of vlotages from 0 to DAC_Vcc (5v)
     * v/MAX_V = x/MAX_DAC
     *
     * v*MAX_DAC/MAX_V
     */
    float dacscaled = voltage*4096.0/5.0;
    uint16_t dacval = dacscaled;

    /* Write DAC */
#ifdef DEBUG_TIME
    now = millis();
#endif
    Serial.println("control 5.5");
    dac.setVoltage(dacval, false);
    Serial.println("control 5.6");
#ifdef DEBUG_TIME
    tell_how_long("set dac", millis()-now);
#endif
    Serial.println("control 6");
}
