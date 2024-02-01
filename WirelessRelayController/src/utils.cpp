#include "utils.h"

void blinkIndicator(int delay_ms /*= 1000*/) {
    long delay_div = (long)(delay_ms / 2);

    digitalWrite(INDICATOR_LED, INDICATOR_LED_ON);
    delay(delay_div);
    digitalWrite(INDICATOR_LED, INDICATOR_LED_OFF);
    delay(delay_div);
}
