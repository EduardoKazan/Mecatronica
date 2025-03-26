#pragma once
#include "controller_keys.h"
#include <Arduino.h>

class Buzzer {
public:
  void init();

  void on();

  void off();

  void changeFreq(uint32_t freq);

  void changeFreqTest();
};
