#include "src/interval.h"
#include "src/interval.c"
#include "src/pulse.h"
#include "src/pulse.c"

typedef uint8_t pin;

const pin trig_pin = 9;
const pin echo_pin = 10;
const pin led_pin = 13;

void toggle_led()
{
    if (digitalRead(led_pin) == HIGH)
    {
        digitalWrite(led_pin, LOW);
    }
    else
    {
        digitalWrite(led_pin, HIGH);
    }
}

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

struct debug_info
{
    unsigned long trigger_count;
    unsigned long echo_timeout;
};

struct distance_measurement dm;
struct debug_info di;
unsigned long next_report = 0;

// Measure distance without blocking.
void measure_distance_async(struct distance_measurement *dm)
{
    if (dm->trigger_start_millis == 0)
    {
        // No measurement in flight, trigger one.
        dm->trigger_start_millis = millis();
        dm->pulse = (struct pulse_measurement){};
        trigger_echo();
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

void trigger_echo()
{
    di.trigger_count++;
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(interval_micros(2 * INTERVAL_MICROS));
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(interval_micros(10 * INTERVAL_MICROS));
    digitalWrite(trig_pin, LOW);
}

void setup()
{
    pinMode(led_pin, OUTPUT);
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    Serial.begin(9600);
}

void loop()
{
    unsigned long now = millis();
    measure_distance_async(&dm);
    if (now > next_report)
    {
        Serial.print("distance cm ");
        Serial.print(dm.distance_cm);
        Serial.print(" width ");
        Serial.print(dm.pulse.width_micros);
        Serial.print(" trigger count ");
        Serial.print(di.trigger_count);
        Serial.print(" timeout ");
        Serial.print(di.echo_timeout);
        Serial.print(" start time ");
        Serial.println(dm.pulse.pulse_started_at_micros);
        next_report = now + interval_millis(1 * INTERVAL_SEC);
        toggle_led();
    }
}
