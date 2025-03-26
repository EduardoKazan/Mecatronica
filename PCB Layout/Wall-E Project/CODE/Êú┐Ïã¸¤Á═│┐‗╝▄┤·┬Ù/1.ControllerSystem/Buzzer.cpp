#include "Buzzer.h"
#include "LED.h"
LED led;
void Buzzer::init() {
  led.init();
  
  //ledcSetup(0, 3000, 12);
  //ledcWrite(0, 0);
  //ledcAttachPin(PIN_BUZZER, 0);
}

void Buzzer::on() {
  ledcWrite(0, pow(2, 10));
}

void Buzzer::off() {
  ledcWrite(0, 0);
}

void Buzzer::changeFreq(uint32_t freq) {
  ledcChangeFrequency(0, freq, 12);
}

void Buzzer::changeFreqTest() {
  ledcWrite(0, pow(2, 10));
  while (digitalRead(PIN_L_UP) || digitalRead(PIN_R_UP)) {
    changeFreq(analogRead(PIN_LX));
    delay(50);
  }
}
