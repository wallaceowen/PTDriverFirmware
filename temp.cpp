#include <stdint.h>

// PT100 conditioner
#include <Adafruit_MAX31865.h>

#include "temp.h"
#include "ports.h"

static float volts[] = {0.7, .78,  .852, .9,   1.0,   1.101, 1.2,   1.3,   1.4,   1.5,   1.603, 1.7,   3.0};
static float temps[] = {0.0, 20.0, 45.0, 62.0, 100.0, 138.0, 176.0, 215.0, 256.0, 297.0, 340.0, 381.0, 500.0 };
#define NUM_TEMPS (sizeof(temps)/sizeof(temps[0]))

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
#define RNOMINAL  100.0

// Use software SPI: CS, DI, DO, CLK
// static Adafruit_MAX31865 thermo = Adafruit_MAX31865(PT100_CS, SPI_MOSI, SPI_MISO, SPI_SCK);
static Adafruit_MAX31865 thermo = Adafruit_MAX31865(PT100_CS);

// look up the temp in the temp table, use lerp against volts table
// to find linear approximation of the right point between two known points
float Temp::compute_voltage(float temp)
{
    for (unsigned i = 0; i < NUM_TEMPS-1; ++i)
        if ((temp >= temps[i]) && (temp < temps[i+1]))
        {
            float tempspan = temps[i+1]-temps[i];
            float delta = temp-temps[i];
            float ratio = delta/tempspan;
            float voltspan = volts[i+1]-volts[i];
            return volts[i]+(voltspan*ratio);
        }

    return 0.0;
}

uint8_t Temp::read_temp(float &temperature)
{
    temperature = thermo.temperature(RNOMINAL, RREF);
    return thermo.readFault();
}

Temp::Temp()
    : m_temp(0.0)
{
    thermo.begin(MAX31865_2WIRE);  // set to 2WIRE or 4WIRE as necessary
}

void Temp::show_temp_fault(uint8_t temp_fault)
{
#ifdef DISP_SHOW_FIXED
    {
        char buffer[32];
        sprintf(buffer, "Fault 0x%x", temp_fault);
        disp_show(0, 0, const_cast<const char*>(buffer));
    }
    if (temp_fault & MAX31865_FAULT_HIGHTHRESH)
        disp_show(1, 0, "High Threshold");
    if (temp_fault & MAX31865_FAULT_LOWTHRESH)
        disp_show(1, 0, "Low Threshold");
    if (temp_fault & MAX31865_FAULT_REFINLOW)
        disp_show(1, 0, "REFIN too big");
    if (temp_fault & MAX31865_FAULT_REFINHIGH)
        disp_show(1, 0, "REFIN open");
    if (temp_fault & MAX31865_FAULT_RTDINLOW)
        disp_show(1, 0, "RTDIN open");
    if (temp_fault & MAX31865_FAULT_OVUV)
        disp_show(1, 0, "Bad voltage");
#else
    char buffer[32];
    sprintf(buffer, "Fault 0x%x", temp_fault);
    Serial.print("Temp ");
    Serial.println(buffer);
#endif
}

#define SCOUNT_MOD 1

void show_temp(float temp)
{
    Serial.print("Temp ");
    Serial.println(temp);
}

void Temp::loop()
{
    static int scount = 0;

    ++scount;
    // if (!(scount%SCOUNT_MOD))
    {
        float temp;
        uint8_t temp_fault = read_temp(temp);
        if (temp_fault)
            show_temp_fault(temp_fault);
        else
        {
            m_temp = temp;
            show_temp(m_temp);
        }
    }

}

    // float temperature;
    // uint8_t temp_fault = read_temp(temperature);
