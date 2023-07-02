#include "interval.h"

inline long interval_millis(interval dt)
{
    return dt / INTERVAL_MILLIS;
}

inline long interval_micros(interval dt)
{
    return dt / INTERVAL_MICROS;
}
