#ifndef __PULSE_H__
#define __PULSE_H__

struct pulse_measurement
{
    unsigned long pulse_started_at_micros;
    unsigned long width_micros;
};

#endif // __PULSE_H__