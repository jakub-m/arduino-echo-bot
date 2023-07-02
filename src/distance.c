#include "distance.h"
#include "interval.h"
#include "Arduino.h"

// Measure distance without blocking.
void measure_distance_async(struct distance_measurement *dm, uint8_t trigger_pin, uint8_t echo_pin)
{
    if (dm->trigger_start_millis == 0)
    {
        // No measurement in flight, trigger one.
        dm->trigger_start_millis = millis();
        dm->pulse = (struct pulse_measurement){};
        trigger_echo(trigger_pin);
        measure_pulse_async(&dm->pulse, echo_pin, HIGH);
    }
    else
    {
        // There is a measurement in flight.
        measure_pulse_async(&dm->pulse, echo_pin, HIGH);
        if (dm->pulse.width_micros == 0)
        {
            if (millis() - dm->trigger_start_millis > interval_millis(10 * INTERVAL_MILLIS))
            {
                // Pulse never came back. Abort and send another one.
                dm->trigger_start_millis = 0;
            }
            else
            {
                // Pulse not yet ended, still waiting.
            }
        }
        else
        {
            dm->trigger_start_millis = 0;
            // Speed of sound is .0343 c/uS, which is 34.3 cm per mS
            dm->distance_cm = 0.0343 * dm->pulse.width_micros / 2;
        }
    }
}

void trigger_echo(uint8_t trigger_pin)
{
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(interval_micros(2 * INTERVAL_MICROS));
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(interval_micros(10 * INTERVAL_MICROS));
    digitalWrite(trigger_pin, LOW);
}
