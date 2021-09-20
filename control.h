
#ifndef __control_h
#define __control_h


// DAC
// #include "dac.h"

// Encoder
#include "encoder.h"

// Display
#include "display.h"

// Temp
#include "temp.h"

class Control
{
public:
	Control(Display *display, Temp *temp, Encoder &encoder);
	void loop();
private:
	friend void render_cb(U8G2 &disp, void *param);
	void render_fields(U8G2 &disp) const;
	float read_offset(void);

    void update_last_params();
    void update_params(float t, float a_temp, float off, float voltage);
    bool params_changed();

	Display    *m_display;
	// DAC        &m_dac;
	Encoder    &m_encoder;
    Temp      *m_temp;

    static float temperature;
    static float adj_temp;
    static float offset;
    static float voltage;

    static float last_temp;
    static float last_offset;
};

#endif
