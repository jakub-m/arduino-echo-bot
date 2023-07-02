typedef uint8_t pin;

/**
 * Interval. The base unit is microsecond, and this can be converted to other units with intervalMillis or intervalMicro.
 */
typedef long interval;

/**
 * Return interval as milliseconds.
 */
inline long interval_millis(interval dt)
{
    return dt / 1000;
}

/**
 * Return interval as microseconds.
 */
inline long interval_micro(interval dt)
{
    return dt;
}

const interval us = 1;
const interval ms = 1000 * us;
const interval sec = 1000 * ms;

const pin trigPin = 9;
const pin echoPin = 10;
const pin ledPin = 13;

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
}

void loop()
{
    float distanceCm = measure_distance_cm();
    Serial.println(distanceCm);
    delay(interval_millis(300 * ms));
    if (digitalRead(ledPin) == HIGH)
    {
        digitalWrite(ledPin, LOW);
    }
    else
    {
        digitalWrite(ledPin, HIGH);
    }
}

float measure_distance_cm()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(interval_micro(2 * us));
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(interval_micro(10 * us));
    digitalWrite(trigPin, LOW);

    unsigned long durationUs = pulseIn(echoPin, HIGH, interval_micro(10 * ms));
    // Speed of sound is .0343 c/uS, which is 34.3 cm per ms
    const float vs_cm_us = 0.0343;
    float distance = vs_cm_us * durationUs / 2;
    return distance;
}
