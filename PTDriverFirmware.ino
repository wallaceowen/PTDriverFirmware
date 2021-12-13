// Include Arduino env (Serial, String, etc)
#include <Arduino.h>

#include <math.h> /* for abs() */

// I2C lib
#include <Wire.h>

// DAC lib
// #include <Adafruit_MCP4725.h>

// PT100 conditioner
#include <Adafruit_MAX31865.h>

#include "vnew.h"
#include "ports.h"
#include "temp.h"
#include "display.h"
#include "control.h"
#include "debug.h"

// Encoder
#include "encoder.h"

// Buttons
#include "buttons.h"

// Display
// The display object buffer (because we have no heap)
static char disp_buffer[sizeof(Display)];
static Display    *display;

// Temp class instance
static char temp_buffer[sizeof(Temp)];
static Temp    *temp;


// Control buffer
// The display object buffer (because we have no heap)
static char control_buffer[sizeof(Control)];
static Control    *control;

static char buttons_buffer[sizeof(Buttons)];
static Buttons    *buttons;

// static DAC dac;
static Encoder encoder;

void setup()
{
    Serial.begin(115200);
    while(!Serial) {} // Wait for serial port

    Serial.println("setup()");
    Wire.begin();
    // dac.begin(DAC_ADDR);

    display = new (disp_buffer) Display();

    temp = new (temp_buffer) Temp();

    buttons = new (buttons_buffer) Buttons();

    control = new (control_buffer) Control(display, temp, encoder, buttons);
}

void loop()
{
// Serial.println("test 1");
#ifdef DEBUG_TIME
    int32_t now = 0;
#endif

    Serial.println("setup()");

#ifdef DEBUG_TIME
    now = millis();
#endif
// Serial.println("test 2");

    temp->loop();
    buttons->loop();
    control->loop();
// Serial.println("test 3");

#ifdef DEBUG_TIME
    tell_how_long("after_c", millis()-now);
#endif
// Serial.println("test 4");

#ifdef DEBUG_TIME
    now = millis();
#endif
// Serial.println("test 5");

    display->loop();
// Serial.println("test 6");

#ifdef DEBUG_TIME
    tell_how_long("after_d", millis()-now);
#endif
// Serial.println("test 7");
}

