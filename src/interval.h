#ifndef __INTERVAL_H__
#define __INTERVAL_H__

/**
 * Interval. The base unit is microsecond, and this can be converted to other units with intervalMillis or intervalMicro.
 */
typedef long interval;

#define INTERVAL_MICROS (1L)
#define INTERVAL_MILLIS (1000L * INTERVAL_MICROS)
#define INTERVAL_SEC (1000L * INTERVAL_MILLIS)

/**
 * Return interval as milliseconds.
 */
long interval_millis(interval dt);

/**
 * Return interval as microseconds.
 */
long interval_micros(interval dt);

#endif // __INTERVAL_H__