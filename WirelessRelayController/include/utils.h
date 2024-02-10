#include <Arduino.h>

// Maintaining the https://en.wikipedia.org/wiki/One_Definition_Rule
#ifndef INDICATOR_LED

#define INDICATOR_LED 2
// INDICATOR_LED and My Relay is inverted; So HIGH Means OFF
#define ON LOW
#define OFF HIGH

// Makes the onboard led blink once
void blinkIndicator(int delay_ms = 1000);

#endif
