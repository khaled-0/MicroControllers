#include <Arduino.h>
#define INDICATOR_LED 2
// INDICATOR_LED is inverted; So ON Means OFF
#define INDICATOR_LED_ON LOW
#define INDICATOR_LED_OFF HIGH
#define ON HIGH
#define OFF LOW

// Makes the onboard led blink once
void blinkIndicator(int delay_ms = 1000);