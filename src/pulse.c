#include "pulse.h"
#include "stdint.h"

/**
 * state is HIGH or LOW.
 */
void measure_pulse_async(struct pulse_measurement *pm, uint8_t pin, uint8_t state)
{
    if (pm->pulse_started_at_micros != 0 && pm->width_micros != 0)
    {
        return;
    }
    uint8_t actual = digitalRead(pin);
    if (pm->pulse_started_at_micros == 0)
    {
        if (actual == state)
        {
            pm->pulse_started_at_micros = micros();
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
            pm->width_micros = micros() - pm->pulse_started_at_micros;
        }
    }
}