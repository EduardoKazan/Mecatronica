#include "Screen.h"

void Screen::init() {
  rm67162_init();
  lcd_setRotation(0);
  spr.createSprite(240, 536, 2);
  spr.setSwapBytes(1);
  spr.fillSprite(TFT_BLACK);
}
