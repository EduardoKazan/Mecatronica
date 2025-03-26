#pragma once
#include "controller_keys.h"
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>

class NRF {
public:
  NRF(int cePin = 43, int csnPin = 42);
  void init(int conID, uint8_t PALevel = 0);
  void testConToCon();
  RF24 radio;
private:
  int _conID;
  byte _addresses[2][6] = { "00000", "00001" };
};
