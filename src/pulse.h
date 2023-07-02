#ifndef __PULSE_H__
#define __PULSE_H__

struct pulse_measurement
{
    unsigned long pulse_started_at_millis;
    unsigned long width_millis;
};

#endif // __PULSE_H__