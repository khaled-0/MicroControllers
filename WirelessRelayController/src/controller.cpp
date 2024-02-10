#include "controller.h"

Controller::Controller(String name, uint8_t GPIO,
                       uint8_t value /*= OFF*/,
                       void (*onChange)(Controller*) /*= nullptr*/)
    : name(name),
      GPIO(GPIO),
      value(value),
      onChange(onChange) {
    pinMode(GPIO, OUTPUT);
    digitalWrite(GPIO, value);
}

String Controller::getValue() {
    return value == ON ? "on" : "off";
}

uint8_t Controller::getValueRaw() {
    return value;
}

void Controller::setValue(uint8_t _value) {
    value = _value;
    digitalWrite(GPIO, value);

    if (onChange != nullptr) onChange(this);
}

void Controller::toggleValue() {
    value = value == ON ? OFF : ON;
    digitalWrite(GPIO, value);

    if (onChange != nullptr) onChange(this);
}
