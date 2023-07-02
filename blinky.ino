typedef uint8_t pin;

/**
 * Interval. The base unit is microsecond, and this can be converted to other units with intervalMillis or intervalMicro.
 */
typedef int interval;

const int ms = 1000;

const pin trigPin = 9;
const pin echoPin = 10;
const pin ledPin = 13;
const interval ledDelay = 1 * ms;

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
}

void loop()
{
    float distanceCm = measureDistanceCm();
    delay(300);
    Serial.println(distanceCm);
}

float measureDistanceCm()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // timeout is in microseconds
    const unsigned long timeoutUs = 1 * 1000 * 1000; // 1 second
    unsigned long durationUs = pulseIn(echoPin, HIGH, timeoutUs);
    // Speed of sound is .0343 c/uS
    const float vs_cm_us = 0.0343;
    float distance = vs_cm_us * durationUs / 2;
    return distance;
}

// digitalWrite(ledPin, HIGH);
// delay(ledDelay);
// digitalWrite(ledPin, LOW);
// delay(ledDelay);