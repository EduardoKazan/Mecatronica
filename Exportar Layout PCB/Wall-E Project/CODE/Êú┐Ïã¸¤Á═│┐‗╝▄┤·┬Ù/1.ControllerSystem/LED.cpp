#include "LED.h"
#include "controller_keys.h"
#include <Arduino.h>

void LED::init() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}

void LED::on() {
  digitalWrite(PIN_LED, HIGH);
}

void LED::off() {
  digitalWrite(PIN_LED, LOW);
}
