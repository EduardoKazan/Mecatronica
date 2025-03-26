#include "NRF.h"

NRF::NRF(int cePin, int csnPin)
  : radio(cePin, csnPin) {
  SPIClass* nrf_spi = new SPIClass(1);
  nrf_spi->begin(12, 13, 11);
  radio.begin(nrf_spi);
}

void NRF::init(int conID, uint8_t PALevel) {
  _conID = conID;
  radio.openWritingPipe(_addresses[_conID]);
  radio.openReadingPipe(1, _addresses[!_conID]);
  radio.setPALevel(PALevel);
}

void NRF::testConToCon() {
  int _s = 0;
  int _r = 0;

  while (true) {
    delay(20);
    radio.startListening();
    if (radio.available()) {
      radio.read(&_r, sizeof(_r));
    }

    digitalWrite(PIN_LED, _r);

    delay(20);
    radio.stopListening();
    _s = digitalRead(PIN_BOARD_R);
    radio.write(&_s, sizeof(_s));

    if (digitalRead(PIN_L_UP) == 0 && digitalRead(PIN_R_UP) == 0) {
      break;
    }
  }
}