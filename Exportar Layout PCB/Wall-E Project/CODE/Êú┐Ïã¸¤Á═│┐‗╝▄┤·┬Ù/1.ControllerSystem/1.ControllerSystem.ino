#include "NRF.h"
#include "Keys.h"
#include "Screen.h"
#include "Bluetooth.h"
#include "WIFI.h"
#include "LED.h"
#include "Buzzer.h"
#include "controller_keys.h"
#include "chinese_32.h"
#include "icons/1nrf.h"
#include "icons/2game.h"
#include "icons/3vs.h"
#include "icons/4info.h"
#include "icons/5ble.h"
#include "icons/6set.h"
#include "icons/1nrf/1pickup.h"
#include "icons/1nrf/2truck.h"
#include "icons/1nrf/3tank.h"
#include "icons/1nrf/4drone.h"
#include "icons/1nrf/5excavator.h"
#include "icons/1nrf/6ship.h"
#include "icons/2game/2_1snake.h"
#include "icons/2game/2_2brick.h"
#include "icons/2game/2_3plane.h"
#include "icons/2game/2_4num2048.h"
#include "icons/2game/2_5tetris.h"
#include "icons/3vs/3_1_ball.h"
#include "icons/4info/4_1_bilibili.h"
#include "icons/4info/4_2_weather.h"
#include "icons/4info/4_3_stock.h"
#include "icons/6set/6_1_keysTest.h"
#include "icons/6set/6_2_cube.h"

int ID = 0;

NRF nrf;
Keys keys;
Screen screen;
Bluetooth bt;
WIFI wifi;
LED led;
Buzzer buzzer;

void menu();
void NRFControl();
void localGame();
void VSGame();
void netInfo();
void btGamepad();
void systemSet();
void pickup();
void truck();
void tank();
void drone();
void excavator();
void ship();
void snake();
void brick();
void plane();
void num2048();
void tetris();
void ball();
void bilibili();
void weather();
void stock();
void keysTest();
void cube();
int getVolADC();
int getVol();
int setGetVolTimer(int time);
void closeGetVolTimer(int timerID);

void setup() {
  Serial.begin(9600);
  nrf.init(ID, 0);
  keys.init(ID);
  screen.init();
  buzzer.init();
  led.init();
  screen.spr.loadFont(chinese_32);
  menu();
}

void loop() {
  delay(1000);
}

void menu() {
  bool pushFlag = true;
  int menuIndex = 0;
  String menuName[6] = { "NRF遥控", "本机游戏", "双人对战", "网络信息", "蓝牙手柄", "系统设置" };
  screen.spr.setTextColor(TFT_WHITE);
  screen.spr.setTextDatum(TC_DATUM);

  while (true) {
    if (keys.left.pressed()) {
      menuIndex--;
      pushFlag = true;
      if (menuIndex < 0) { menuIndex = 5; }
    } else if (keys.right.pressed()) {
      menuIndex++;
      pushFlag = true;
      if (menuIndex > 5) { menuIndex = 0; }
    }

    if (pushFlag) {
      screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);
      screen.spr.drawString(menuName[menuIndex], 120, 150, 4);
      screen.spr.fillSmoothRoundRect(20, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothRoundRect(56, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothRoundRect(92, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothRoundRect(128, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothRoundRect(164, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothRoundRect(200, 450, 20, 12, 3, TFT_VIOLET, TFT_BLACK);

      if (menuIndex == 0) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_1nrf);
        screen.spr.fillSmoothRoundRect(20, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK);
      }
      if (menuIndex == 1) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_2game);
        screen.spr.fillSmoothRoundRect(56, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK);
      }
      if (menuIndex == 2) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_3vs);
        screen.spr.fillSmoothRoundRect(92, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK);
      }
      if (menuIndex == 3) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_4info);
        screen.spr.fillSmoothRoundRect(128, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK);
      }
      if (menuIndex == 4) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_5ble);
        screen.spr.fillSmoothRoundRect(164, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK);
      }
      if (menuIndex == 5) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_6set);
        screen.spr.fillSmoothRoundRect(200, 450, 20, 12, 3, TFT_CYAN, TFT_BLACK);
      }

      lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());
      pushFlag = false;
    }

    if (keys.o.pressed()) {
      switch (menuIndex) {
        case 0:
          NRFControl();
          pushFlag = true;
          break;
        case 1:
          localGame();
          pushFlag = true;
          break;
        case 2:
          VSGame();
          pushFlag = true;
          break;
        case 3:
          netInfo();
          pushFlag = true;
          break;
        case 4:
          btGamepad();
          pushFlag = true;
          break;
        case 5:
          systemSet();
          pushFlag = true;
          break;
        default:
          break;
      }
    }

    delay(10);
  }
}

void NRFControl() {
  bool pushFlag = true;
  int nrfIndex = 0;
  String nrfName[6] = { "皮卡", "货车", "坦克", "无人机", "挖掘机", "舰船" };
  screen.spr.setTextColor(TFT_WHITE);
  screen.spr.setTextDatum(TC_DATUM);

  while (true) {
    if (keys.left.pressed()) {
      nrfIndex--;
      pushFlag = true;
      if (nrfIndex < 0) { nrfIndex = 5; }
    } else if (keys.right.pressed()) {
      nrfIndex++;
      pushFlag = true;
      if (nrfIndex > 5) { nrfIndex = 0; }
    }

    if (pushFlag) {
      screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      screen.spr.drawString(nrfName[nrfIndex], 120, 150, 4);
      screen.spr.fillSmoothCircle(30, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(66, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(102, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(138, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(174, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(210, 456, 6, TFT_VIOLET, TFT_BLACK);

      if (nrfIndex == 0) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_1_1pickup);
        screen.spr.fillSmoothCircle(30, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (nrfIndex == 1) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_1_2truck);
        screen.spr.fillSmoothCircle(66, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (nrfIndex == 2) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_1_3tank);
        screen.spr.fillSmoothCircle(102, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (nrfIndex == 3) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_1_4drone);
        screen.spr.fillSmoothCircle(138, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (nrfIndex == 4) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_1_5excavator);
        screen.spr.fillSmoothCircle(174, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (nrfIndex == 5) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_1_6ship);
        screen.spr.fillSmoothCircle(210, 456, 6, TFT_CYAN, TFT_BLACK);
      }

      lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());
      pushFlag = false;
    }

    if (keys.o.pressed()) {
      switch (nrfIndex) {
        case 0:
          pickup();
          pushFlag = true;
          break;
        case 1:
          truck();
          pushFlag = true;
          break;
        case 2:
          tank();
          pushFlag = true;
          break;
        case 3:
          drone();
          pushFlag = true;
          break;
        case 4:
          excavator();
          pushFlag = true;
          break;
        case 5:
          ship();
          pushFlag = true;
          break;
        default:
          break;
      }
    } else if (keys.x.pressed()) {
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      Serial.println("返回主菜单");
      break;
    }

    delay(10);
  }
}

void localGame() {
  bool gameFlag = true;
  int gameIndex = 0;
  String gameName[5] = { "贪吃蛇", "打砖块", "飞机大战", "2048", "俄罗斯方块" };
  screen.spr.setTextColor(TFT_WHITE);
  screen.spr.setTextDatum(TC_DATUM);

  while (true) {
    if (keys.left.pressed()) {
      gameIndex--;
      gameFlag = true;
      if (gameIndex < 0) { gameIndex = 4; }
    } else if (keys.right.pressed()) {
      gameIndex++;
      gameFlag = true;
      if (gameIndex > 4) { gameIndex = 0; }
    }

    if (gameFlag) {
      screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      screen.spr.drawString(gameName[gameIndex], 120, 150, 4);
      screen.spr.fillSmoothCircle(48, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(84, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(120, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(156, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(192, 456, 6, TFT_VIOLET, TFT_BLACK);

      if (gameIndex == 0) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_2_1snake);
        screen.spr.fillSmoothCircle(48, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (gameIndex == 1) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_2_2brick);
        screen.spr.fillSmoothCircle(84, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (gameIndex == 2) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_2_3plane);
        screen.spr.fillSmoothCircle(120, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (gameIndex == 3) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_2_4num2048);
        screen.spr.fillSmoothCircle(156, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (gameIndex == 4) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_2_5tetris);
        screen.spr.fillSmoothCircle(192, 456, 6, TFT_CYAN, TFT_BLACK);
      }

      lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());
      gameFlag = false;
    }

    if (keys.o.pressed()) {
      switch (gameIndex) {
        case 0:
          snake();
          gameFlag = true;
          break;
        case 1:
          brick();
          gameFlag = true;
          break;
        case 2:
          plane();
          gameFlag = true;
          break;
        case 3:
          num2048();
          gameFlag = true;
          break;
        case 4:
          tetris();
          gameFlag = true;
          break;
        default:
          break;
      }
    } else if (keys.x.pressed()) {
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      Serial.println("返回主菜单");
      break;
    }

    delay(10);
  }
}

void VSGame() {
  bool vsFlag = true;
  int vsIndex = 0;
  String vsName[1] = { "弹球" };
  screen.spr.setTextColor(TFT_WHITE);
  screen.spr.setTextDatum(TC_DATUM);

  while (true) {
    if (keys.left.pressed()) {
      vsIndex--;
      vsFlag = true;
      if (vsIndex < 0) { vsIndex = 0; }
    } else if (keys.right.pressed()) {
      vsIndex++;
      vsFlag = true;
      if (vsIndex > 0) { vsIndex = 0; }
    }

    if (vsFlag) {
      screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      screen.spr.drawString(vsName[vsIndex], 120, 150, 4);
      screen.spr.fillSmoothCircle(120, 456, 6, TFT_VIOLET, TFT_BLACK);

      if (vsIndex == 0) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_3_1_ball);
        screen.spr.fillSmoothCircle(120, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());
      vsFlag = false;
    }

    if (keys.o.pressed()) {
      switch (vsIndex) {
        case 0:
          ball();
          vsFlag = true;
          break;
        default:
          break;
      }
    } else if (keys.x.pressed()) {
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      Serial.println("返回主菜单");
      break;
    }

    delay(10);
  }
}

void netInfo() {
  bool infoFlag = true;
  int infoIndex = 0;
  String infoName[3] = { "哔哩哔哩", "天气预报", "股票基金" };
  screen.spr.setTextColor(TFT_WHITE);
  screen.spr.setTextDatum(TC_DATUM);

  while (true) {
    if (keys.left.pressed()) {
      infoIndex--;
      infoFlag = true;
      if (infoIndex < 0) { infoIndex = 2; }
    } else if (keys.right.pressed()) {
      infoIndex++;
      infoFlag = true;
      if (infoIndex > 2) { infoIndex = 0; }
    }

    if (infoFlag) {
      screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      screen.spr.drawString(infoName[infoIndex], 120, 150, 4 screen.spr.fillSmoothCircle(84, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(120, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(156, 456, 6, TFT_VIOLET, TFT_BLACK);

      if (infoIndex == 0) {
        screen.spr.fillRect(0, 400, 240, 20, TFT_BLACK);
        screen.spr.pushImage(20, 220, 200, 181, image_data_4_1_bilibili);
        screen.spr.fillSmoothCircle(84, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (infoIndex == 1) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_4_2_weather);
        screen.spr.fillSmoothCircle(120, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (infoIndex == 2) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_4_3_stock);
        screen.spr.fillSmoothCircle(156, 456, 6, TFT_CYAN, TFT_BLACK);
      }

      lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());
      infoFlag = false;
    }

    if (keys.o.pressed()) {
      switch (infoIndex) {
        case 0:
          bilibili();
          infoFlag = true;
          break;
        case 1:
          weather();
          infoFlag = true;
          break;
        case 2:
          stock();
          infoFlag = true;
          break;
        default:
          break;
      }
    } else if (keys.x.pressed()) {
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      Serial.println("返回主菜单");
      break;
    }

    delay(10);
  }
}

void btGamepad() {}

void systemSet() {
  bool setFlag = true;
  int setIndex = 0;
  String setName[2] = { "按键测试", "陀螺仪立方体" };
  screen.spr.setTextColor(TFT_WHITE);
  screen.spr.setTextDatum(TC_DATUM);

  while (true) {
    if (keys.left.pressed()) {
      setIndex--;
      setFlag = true;
      if (setIndex < 0) { setIndex = 1; }
    } else if (keys.right.pressed()) {
      setIndex++;
      setFlag = true;
      if (setIndex > 1) { setIndex = 0; }
    }

    if (setFlag) {
      screen.spr.fillRect(0, 145, 240, 40, TFT_BLACK);
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      screen.spr.drawString(setName[setIndex], 120, 150, 4);
      screen.spr.fillSmoothCircle(102, 456, 6, TFT_VIOLET, TFT_BLACK);
      screen.spr.fillSmoothCircle(138, 456, 6, TFT_VIOLET, TFT_BLACK);

      if (setIndex == 0) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_6_1_keysTest);
        screen.spr.fillSmoothCircle(102, 456, 6, TFT_CYAN, TFT_BLACK);
      }
      if (setIndex == 1) {
        screen.spr.pushImage(20, 220, 200, 200, image_data_6_2_cube);
        screen.spr.fillSmoothCircle(138, 456, 6, TFT_CYAN, TFT_BLACK);
      }

      lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());
      setFlag = false;
    }

    if (keys.o.pressed()) {
      switch (setIndex) {
        case 0:
          keysTest();
          setFlag = true;
          break;
        case 1:
          cube();
          setFlag = true;
          break;
        default:
          break;
      }
    } else if (keys.x.pressed()) {
      screen.spr.fillRect(0, 440, 240, 25, TFT_BLACK);
      Serial.println("返回主菜单");
      break;
    }

    delay(10);
  }
}

void pickup() {
  Serial.println("1.1 皮卡");
}

void truck() {
  Serial.println("1.2 货车");
}

void tank() {
  Serial.println("1.3 坦克");
  struct TANK_KVS {
    bool light = 0;
    bool power = 0;
    bool trigger = 0;
    int8_t LX = 0;
    int8_t LY = 0;
    int8_t RX = 0;
    int8_t RY = 0;
  };
  TANK_KVS tank_kvs;
  const byte address[6] = "00001";
  nrf.radio.openWritingPipe(address);
  nrf.radio.setPALevel(RF24_PA_MIN);
  nrf.radio.stopListening();
  screen.spr.fillSprite(TFT_BLACK);
  screen.spr.setTextDatum(TL_DATUM);
  while (true) {
    keys.kvs_update();
    tank_kvs.LX = keys.kvs.LX;
    tank_kvs.LY = keys.kvs.LY;
    tank_kvs.RX = keys.kvs.RX;
    tank_kvs.RY = keys.kvs.RY;
    tank_kvs.light = keys.kvs.switch_L1;
    tank_kvs.power = keys.kvs.switch_R2;
    tank_kvs.trigger = keys.kvs.R_up;
    nrf.radio.write(&tank_kvs, sizeof(tank_kvs));
    delay(20);
    screen.spr.fillSprite(TFT_BLACK);
    screen.spr.drawString("LX: " + String(tank_kvs.LX), 30, 20);
    screen.spr.drawString("LY: " + String(tank_kvs.LY), 30, 60);
    screen.spr.drawString("RX: " + String(tank_kvs.RX), 30, 100);
    screen.spr.drawString("RY: " + String(tank_kvs.RY), 30, 140);
    screen.spr.drawString("light: " + String(tank_kvs.light), 30, 180);
    screen.spr.drawString("power: " + String(tank_kvs.power), 30, 220);
    screen.spr.drawString("trigger: " + String(!tank_kvs.trigger), 30, 260);
    screen.spr.drawString("QUIT: X键", 30, 400);
    lcd_PushColors(0, 0, screen.spr.width(), screen.spr.height(), (uint16_t*)screen.spr.getPointer());
    if (keys.x.pressed()) {
      screen.spr.fillSprite(TFT_BLACK);
      screen.spr.setTextDatum(TC_DATUM);
      break;
    }
  }
}

void drone() {
  Serial.println("1.4 无人机");
}

void excavator() {
  Serial.println("1.5 挖掘机");
}

void ship() {
  Serial.println("1.6 舰船");
}

void snake() {
  Serial.println("2.1 贪吃蛇");
}

void brick() {
  Serial.println("2.2 打砖块");
}

void plane() {
  Serial.println("2.3 飞机大战");
}

void num2048() {
  Serial.println("2.4 2048");
}

void tetris() {
  Serial.println("2.5 俄罗斯方块");
}

void ball() {
  Serial.println("3.1 弹球");
}

void bilibili() {
  Serial.println("4.1 哔哩哔哩");
}

void weather() {
  Serial.println("4.2 天气预报");
}

void stock() {
  Serial.println("4.3 股票基金");
}

void keysTest() {
  {
    screen.spr.fillSprite(TFT_BLACK);
    screen.spr.drawString("按键测试", 120, 20, 4);
    screen.spr.drawSmoothCircle(35, 75, 15, TFT_GREEN, TFT_BLACK);
    screen.spr.drawSmoothCircle(80, 70, 10, TFT_GREEN, TFT_BLACK);
    screen.spr.drawSmoothCircle(150, 70, 10, TFT_GREEN, TFT_BLACK);
    screen.spr.drawSmoothCircle(205, 75, 15, TFT_GREEN, TFT_BLACK);
    screen.spr.drawRect(35, 110, 75, 20, TFT_CYAN);
    screen.spr.drawRect(130, 110, 75, 20, TFT_CYAN);
    screen.spr.drawRect(20, 150, 20, 30, TFT_PINK);
    screen.spr.drawRect(60, 150, 20, 30, TFT_PINK);
    screen.spr.drawRect(160, 150, 20, 30, TFT_PINK);
    screen.spr.drawRect(200, 150, 20, 30, TFT_PINK);
    screen.spr.drawRect(10, 200, 100, 100, TFT_CYAN);
    screen.spr.drawRect(130, 200, 100, 100, TFT_CYAN);
    screen.spr.drawRect(80, 320, 20, 10, TFT_WHITE);
    screen.spr.drawRect(140, 320, 20, 10, TFT_WHITE);
    screen.spr.drawRect(40, 360, 20, 20, TFT_SILVER);
    screen.spr.drawRect(40, 400, 20, 20, TFT_SILVER);
    screen.spr.drawRect(10, 380, 20, 20, TFT_SILVER);
    screen.spr.drawRect(70, 380, 20, 20, TFT_SILVER);
    screen.spr.drawRect(180, 360, 20, 20, TFT_SILVER);
    screen.spr.drawRect(180, 400, 20, 20, TFT_SILVER);
    screen.spr.drawRect(150, 380, 20, 20, TFT_SILVER);
    screen.spr.drawRect(210, 380, 20, 20, TFT_SILVER);
    screen.spr.drawRect(75, 440, 90, 90, TFT_VIOLET);
    lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
  }
  while (true) {
    keys.kvs_update();
    keys.kvs.L_up == 0 ? screen.spr.fillSmoothCircle(35, 75, 12, TFT_GREEN, TFT_BLACK) : screen.spr.fillSmoothCircle(35, 75, 12, TFT_BLACK, TFT_BLACK);
    keys.kvs.L_down == 0 ? screen.spr.fillSmoothCircle(80, 70, 7, TFT_GREEN, TFT_BLACK) : screen.spr.fillSmoothCircle(80, 70, 7, TFT_BLACK, TFT_BLACK);
    keys.kvs.R_down == 0 ? screen.spr.fillSmoothCircle(150, 70, 7, TFT_GREEN, TFT_BLACK) : screen.spr.fillSmoothCircle(150, 70, 7, TFT_BLACK, TFT_BLACK);
    keys.kvs.R_up == 0 ? screen.spr.fillSmoothCircle(205, 75, 12, TFT_GREEN, TFT_BLACK) : screen.spr.fillSmoothCircle(205, 75, 12, TFT_BLACK, TFT_BLACK);
    screen.spr.fillRect(36, 111, 73, 18, TFT_BLACK);
    screen.spr.fillRect(131, 111, 73, 18, TFT_BLACK);
    screen.spr.fillRect(map(keys.kvs.L_knob, -100, 100, 37, 108) - 2, 111, 4, 18, TFT_CYAN);
    screen.spr.fillRect(map(keys.kvs.R_knob, -100, 100, 132, 203) - 2, 111, 4, 18, TFT_CYAN);
    if (keys.kvs.switch_L1 == 1) {
      screen.spr.fillRect(21, 151, 18, 13, TFT_PINK);
      screen.spr.fillRect(21, 166, 18, 13, TFT_BLACK);
      buzzer.on();
      led.on();
    } else {
      screen.spr.fillRect(21, 151, 18, 13, TFT_BLACK);
      screen.spr.fillRect(21, 166, 18, 13, TFT_PINK);
      buzzer.off();
      led.off();
    }
    if (keys.kvs.switch_L2 == 1) {
      screen.spr.fillRect(61, 151, 18, 13, TFT_PINK);
      screen.spr.fillRect(61, 166, 18, 13, TFT_BLACK);
    } else {
      screen.spr.fillRect(61, 151, 18, 13, TFT_BLACK);
      screen.spr.fillRect(61, 166, 18, 13, TFT_PINK);
    }
    if (keys.kvs.switch_R1 == 1) {
      screen.spr.fillRect(161, 151, 18, 13, TFT_PINK);
      screen.spr.fillRect(161, 166, 18, 13, TFT_BLACK);
    } else {
      screen.spr.fillRect(161, 151, 18, 13, TFT_BLACK);
      screen.spr.fillRect(161, 166, 18, 13, TFT_PINK);
    }
    if (keys.kvs.switch_R2 == 1) {
      screen.spr.fillRect(201, 151, 18, 13, TFT_PINK);
      screen.spr.fillRect(201, 166, 18, 13, TFT_BLACK);
    } else {
      screen.spr.fillRect(201, 151, 18, 13, TFT_BLACK);
      screen.spr.fillRect(201, 166, 18, 13, TFT_PINK);
    }
    screen.spr.fillRect(11, 201, 98, 98, TFT_BLACK);
    screen.spr.fillRect(131, 201, 98, 98, TFT_BLACK);
    screen.spr.fillSmoothCircle(10 + map(keys.kvs.LX, -100, 100, 10, 90), 200 + map(keys.kvs.LY, -100, 100, 10, 90), 8, TFT_CYAN, TFT_BLACK);
    screen.spr.fillSmoothCircle(130 + map(keys.kvs.RX, -100, 100, 10, 90), 200 + map(keys.kvs.RY, -100, 100, 10, 90), 8, TFT_CYAN, TFT_BLACK);
    keys.kvs.board_L == 0 ? screen.spr.fillRect(81, 321, 18, 8, TFT_WHITE) : screen.spr.fillRect(81, 321, 18, 8, TFT_BLACK);
    keys.kvs.board_R == 0 ? screen.spr.fillRect(141, 321, 18, 8, TFT_WHITE) : screen.spr.fillRect(141, 321, 18, 8, TFT_BLACK);
    keys.kvs.up == 0 ? screen.spr.fillRect(41, 361, 18, 18, TFT_SILVER) : screen.spr.fillRect(41, 361, 18, 18, TFT_BLACK);
    keys.kvs.down == 0 ? screen.spr.fillRect(41, 401, 18, 18, TFT_SILVER) : screen.spr.fillRect(41, 401, 18, 18, TFT_BLACK);
    keys.kvs.left == 0 ? screen.spr.fillRect(11, 381, 18, 18, TFT_SILVER) : screen.spr.fillRect(11, 381, 18, 18, TFT_BLACK);
    keys.kvs.right == 0 ? screen.spr.fillRect(71, 381, 18, 18, TFT_SILVER) : screen.spr.fillRect(71, 381, 18, 18, TFT_BLACK);
    keys.kvs.x == 0 ? screen.spr.fillRect(181, 361, 18, 18, TFT_SILVER) : screen.spr.fillRect(181, 361, 18, 18, TFT_BLACK);
    keys.kvs.a == 0 ? screen.spr.fillRect(181, 401, 18, 18, TFT_SILVER) : screen.spr.fillRect(181, 401, 18, 18, TFT_BLACK);
    keys.kvs.o == 0 ? screen.spr.fillRect(151, 381, 18, 18, TFT_SILVER) : screen.spr.fillRect(151, 381, 18, 18, TFT_BLACK);
    keys.kvs.b == 0 ? screen.spr.fillRect(211, 381, 18, 18, TFT_SILVER) : screen.spr.fillRect(211, 381, 18, 18, TFT_BLACK);
    int angleX = map(keys.kvs.angleX, -100, 100, 446, 524);
    int angleY = map(keys.kvs.angleY, -100, 100, 81, 159);
    screen.spr.fillRect(76, 441, 88, 88, TFT_BLACK);
    screen.spr.fillRect(angleY - 5, angleX - 5, 10, 10, TFT_VIOLET);
    lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
    if (keys.b.pressed() && keys.x.pressed()) {
      screen.spr.fillSprite(TFT_BLACK);
      lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
      break;
    }
    delay(1);
  }
}

void cube() {
  Serial.println("6.2 陀螺仪立方体");
}

int getVolADC() {
  return analogRead(PIN_VOL);
}

int getVol() {
  return analogReadMilliVolts(PIN_VOL);
}