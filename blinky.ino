#include "src/interval.h"
#include "src/interval.c"
#include "src/pulse.h"
#include "src/pulse.c"
#include "src/distance.h"
#include "src/distance.c"

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

struct distance_measurement dm;
unsigned long next_report = 0;

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
    measure_distance_async(&dm, trig_pin, echo_pin);
    if (now > next_report)
    {
        Serial.print("distance cm ");
        Serial.print(dm.distance_cm);
        Serial.print(" width ");
        Serial.print(dm.pulse.width_micros);
        Serial.print(" start time ");
        Serial.println(dm.pulse.pulse_started_at_micros);
        next_report = now + interval_millis(1 * INTERVAL_SEC);
        toggle_led();
    }
}
