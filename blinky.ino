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
    // Is there a measurement in flight?
    bool in_flight;
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
    if (dm->in_flight)
    {
        measure_pulse_async(&dm->pulse, echo_pin, HIGH);
        if (dm->pulse.width_micros != 0)
        {
            dm->in_flight = false;
            dm->distance_cm = dm->pulse.width_micros;
            delay(500);
            Serial.println(dm->pulse.width_micros);
        }
        //        if (digitalRead(echo_pin) == LOW)
        //        {
        //            unsigned long now_micros = micros();
        //            dm->in_flight = false;
        //            unsigned long dt_micros = now_micros - dm->start_micros;
        //            // Speed of sound is .0343 c/uS, which is 34.3 cm per mS
        //            dm->distance_cm = 0.0343 * dt_micros / 2;
        //        }
        //        else
        //        {
        //            // Potential BUG. Will freeze if the measurement never ends.
        //
        //            //     int max_wait_time = 10 * INTERVAL_MILLIS;
        //            //     // Break measurement when the puls never came back.
        //            //     if (dt_micros > interval_micros(max_wait_time))
        //            //     {
        //            //         di.echo_timeout++;
        //            //         dm->in_flight = false;
        //            //     }
        //        }
    }
    else
    {
        // No measurement in flight, trigger one.
        // dm->start_micros = micros();
        dm->in_flight = true;
        dm->pulse = (struct pulse_measurement){};
        trigger_echo();
    }
}

// float measure_distance_cm()
//{
//     trigger_echo();
//
//     unsigned long durationUs = pulseIn(echo_pin, HIGH, interval_micros(10 * ms));
//     // Speed of sound is .0343 c/uS, which is 34.3 cm per ms
//     const float vs_cm_us = 0.0343;
//     float distance = vs_cm_us * durationUs / 2;
//     return distance;
// }

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
    // unsigned long now = millis();
    measure_distance_async(&dm);
    // if (now > next_report)
    // {
    // Serial.print("distance cm ");
    // Serial.print(dm.distance_cm);
    // Serial.print(" trigger count ");
    // Serial.print(di.trigger_count);
    // Serial.print(" timeout ");
    // Serial.print(di.echo_timeout);
    // Serial.print(" start time ");
    // Serial.println(dm.start_micros);
    // next_report = now + interval_millis(1 * INTERVAL_SEC);
    // toggle_led();
    // }

    // trigger_echo();
    // Serial.println("trigger");
    // unsigned long dt = pulseIn(echo_pin, HIGH);
    // Serial.print("pulse ");
    // Serial.println(dt);
    // delay(interval_millis(300 * INTERVAL_MILLIS));
    // Serial.println("done delay");
}
