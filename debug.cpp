#include <Arduino.h>

#include "debug.h"

void tell_how_long(const char *txt, int32_t dur)
{
    Serial.print(txt);
    Serial.print(": ");
    Serial.println(dur);
}

