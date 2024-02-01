#include <Arduino.h>

#define LED 2

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.println("LED is on");
    digitalWrite(LED, HIGH);

    delay(500);

    Serial.println("LED is off");
    digitalWrite(LED, LOW);

    delay(500);
}