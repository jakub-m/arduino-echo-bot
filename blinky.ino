#include "src/interval.h"

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
    unsigned long start_micros;
    // Last measurement in cm.
    float distance_cm;
    // Is there a measurement in flight?
    bool in_flight;
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
    unsigned long now_micros = micros();
    if (dm->in_flight)
    {
        unsigned long pulse = pulseIn(echo_pin, HIGH); // remove this, do it on my own
        unsigned long dt_micros = now_micros - dm->start_micros;
        Serial.print("pulse ");
        Serial.println(pulse);
        Serial.print("dt_micros ");
        Serial.println(dt_micros);

        if (digitalRead(echo_pin) == HIGH)
        {
            dm->in_flight = false;
            // Speed of sound is .0343 c/uS, which is 34.3 cm per mS
            dm->distance_cm = 0.0343 * dt_micros / 2;
        }
        else
        {
            int max_wait_time = 10 * INTERVAL_MS;
            // Break measurement when the puls never came back.
            if (dt_micros > interval_micros(max_wait_time))
            {
                di.echo_timeout++;
                dm->in_flight = false;
            }
        }
    }
    else
    {
        // No measurement in flight, trigger one.
        dm->start_micros = micros();
        dm->in_flight = true;
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
    delayMicroseconds(interval_micros(2 * INTERVAL_US));
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(interval_micros(10 * INTERVAL_US));
    digitalWrite(trig_pin, LOW);
}

void setup()
{
    pinMode(led_pin, OUTPUT);
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    Serial.begin(9600);
    dm.distance_cm = 0;
    dm.in_flight = false;
    di.trigger_count = 0;
    di.echo_timeout = 0;
}

void loop()
{
    unsigned long now = millis();
    measure_distance_async(&dm);
    if (now > next_report)
    {
        Serial.print("distance cm ");
        Serial.println(dm.distance_cm);
        Serial.print("trigger count ");
        Serial.println(di.trigger_count);
        Serial.print("timeout ");
        Serial.println(di.echo_timeout);
        Serial.print("start time ");
        Serial.println(dm.start_micros);
        // Serial.println(300);
        next_report = now + interval_millis(1 * INTERVAL_SEC);
        toggle_led();
    }

    //    float distanceCm = measure_distance_cm();
    //    Serial.println(distanceCm);
    //    delay(interval_millis(300 * ms));
}
