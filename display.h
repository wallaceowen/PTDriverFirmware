// display.h

#ifndef __display_h
#define __display_h

#include <stdint.h>
#include <U8g2lib.h>  // LCD driver library

// #include "constants.h"

typedef void (*RenderCallback)(U8G2*, void*);

class Display
{
public:
    enum DState { DS_Init, DS_While, DS_WaitForUpdate };

    Display();

    void set_cb(RenderCallback, void*);

    // Call often to run the display state machine (once per main loop)
    void loop();

    // Return true when display is ready for an update (it has
    // finished rendering)
    bool ready() const { return m_state == DS_WaitForUpdate; }

    // Fetch the display's state
    DState state() const { return m_state; }

    void update() { m_updated = true; }

private:

    void render();

    DState m_state;
    bool m_updated;
    RenderCallback m_render_cb;
    void * m_render_param;
};

#endif
