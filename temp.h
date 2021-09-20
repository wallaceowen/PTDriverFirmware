#ifndef __temp_h
#define __temp_h

// look up the temp in the temp table, use lerp against volts table
// to find linear approximation of the right point between two known points

class Temp
{
public:
    Temp();
    void loop();
    static float compute_voltage(float temp);
    float get_temp(unsigned which = 0) const { return m_temp; }
private:
    static uint8_t read_temp(float &temperature);
    static void show_temp_fault(uint8_t temp_fault);
    float m_temp;
};

#endif
