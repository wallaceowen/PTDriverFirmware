
#ifndef __control_h
#define __control_h


// DAC
// #include "dac.h"

// Encoder
#include "encoder.h"

// Display
#include "display.h"

// Buttons
#include "buttons.h"

// Temp
#include "temp.h"

class Control
{
public:
    enum ControlState { CS_Status=0, CS_ThermSet, CS_Save, Num_CS };

	Control(Display *display, Temp *temp, Encoder &encoder, Buttons *buttons);
	void loop();
private:
	friend void render_cb(U8G2 *disp, void *param);
    void render_current(U8G2 *disp);
	void render_fields(U8G2 *disp);
    void show_therm_menu(U8G2 *disp);
    void show_save_menu(U8G2 *disp);

	float read_offset(void);

    void update_last_params();
    void update_params(float t, float a_temp, float off, float voltage);
    bool params_changed();

	friend void buttons_cb(int button, bool state, void *arg);
	void handle_buttons(int button, bool state);

    uint16_t    m_state;
    bool        m_dirty;
	Display    *m_display;
	// DAC        &m_dac;
	Encoder    &m_encoder;
    Temp       *m_temp;
    Buttons    *m_buttons;

    static float temperature;
    static float adj_temp;
    static float offset;
    static float voltage;

    static float last_temp;
    static float last_offset;
};

#endif
