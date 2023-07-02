#include "pulse.h"
#include "stdint.h"

/**
 * state is HIGH or LOW.
 */
void measure_pulse_async(struct pulse_measurement *pm, uint8_t pin, uint8_t state)
{
    if (pm->pulse_started_at_millis != 0 && pm->width_millis != 0)
    {
        return;
    }
    uint8_t actual = digitalRead(pin);
    if (pm->pulse_started_at_millis == 0)
    {
        if (actual == state)
        {
            unsigned long now = millis();
            pm->pulse_started_at_millis = now;
        }
        else
        {
            // Pulse not yet started.
        }
    }
    else
    {
        if (actual == state)
        {
            // Pulse stll in progress.
        }
        else
        {
            unsigned long now = millis();
            pm->width_millis = now - pm->pulse_started_at_millis;
        }
    }
}