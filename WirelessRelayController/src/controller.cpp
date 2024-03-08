#include "controller.h"

#include <EEPROM.h>

Controller::Controller(int index, String name, uint8_t GPIO,
                       uint8_t value /*= OFF*/,
                       void (*onChange)(Controller*) /*= nullptr*/,
                       boolean presist_value /*= true*/)
    : index(index),
      name(name),
      GPIO(GPIO),
      value(value),
      onChange(onChange),
      presist_value(presist_value) {
    pinMode(Controller::GPIO, OUTPUT);

    if (presist_value) Controller::value = loadPresistentValue();
    digitalWrite(Controller::GPIO, Controller::value);
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

    if (presist_value) savePresistentValue();
    if (onChange != nullptr) onChange(this);
}

void Controller::toggleValue() {
    value = value == ON ? OFF : ON;
    digitalWrite(GPIO, value);

    if (presist_value) savePresistentValue();
    if (onChange != nullptr) onChange(this);
}

void Controller::savePresistentValue() {
    EEPROM.write(index * sizeof(uint8_t), value);
    EEPROM.commit();
}

uint8_t Controller::loadPresistentValue() {
    uint8_t eeprom_value = EEPROM.read(index * sizeof(uint8_t));
    return eeprom_value;
}