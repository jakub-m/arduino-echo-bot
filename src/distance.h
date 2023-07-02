#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include "pulse.h"
#include "stdint.h"

// A structure to manage asynchronous measurement.
struct distance_measurement
{
    // Start of the measurement, elapsed microseconds.
    // unsigned long start_micros;
    // Last measurement in cm.
    float distance_cm;
    // If trigger_start_millis is 0 it means that there is no measurement in flight.
    unsigned long trigger_start_millis;
    struct pulse_measurement pulse;
};

void measure_distance_async(struct distance_measurement *dm, uint8_t trigger_pin, uint8_t echo_pin);
void trigger_echo(uint8_t trigger_pin);

#endif