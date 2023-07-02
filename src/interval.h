#ifndef __INTERVAL_H__
#define __INTERVAL_H__

/**
 * Interval. The base unit is microsecond, and this can be converted to other units with intervalMillis or intervalMicro.
 */
typedef long interval;

#define INTERVAL_US (1)
#define INTERVAL_MS (1000 * INTERVAL_US)
#define INTERVAL_SEC (1000 * INTERVAL_MS)

/**
 * Return interval as milliseconds.
 */
long interval_millis(interval dt);

/**
 * Return interval as microseconds.
 */
long interval_micros(interval dt);

#endif // __INTERVAL_H__