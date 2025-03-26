#pragma once

#include <Arduino.h>
#include "controller_keys.h"
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <MCP23017.h>
#include <Bounce2.h>

struct KVS {
  bool L_up = 1;
  bool L_down = 1;
  bool R_up = 1;
  bool R_down = 1;
  int8_t L_knob = 0;
  int8_t LX = 0;
  int8_t LY = 0;
  int8_t R_knob = 0;
  int8_t RX = 0;
  int8_t RY = 0;
  bool board_L = 1;
  bool board_R = 1;
  bool up = 1;
  bool down = 1;
  bool left = 1;
  bool right = 1;
  bool o = 1;
  bool x = 1;
  bool a = 1;
  bool b = 1;
  bool switch_L1 = 1;
  bool switch_L2 = 1;
  bool switch_R1 = 1;
  bool switch_R2 = 1;
  int8_t angleX = 0;
  int8_t angleY = 0;
};

class MCP_bounce {
public:
  MCP_bounce(uint8_t pin, bool default_state);
  bool pressed();
  bool read();
  bool on();
  bool off();

private:
  uint8_t _pin;
  bool _default_state;
  bool _isPressed_flag = false;
};

class Keys {
public:
  MCP_bounce up;
  MCP_bounce down;
  MCP_bounce left;
  MCP_bounce right;
  MCP_bounce o;
  MCP_bounce x;
  MCP_bounce a;
  MCP_bounce b;
  MCP_bounce L1;
  MCP_bounce L2;
  MCP_bounce R1;
  MCP_bounce R2;
  Bounce2::Button L_up;
  Bounce2::Button L_down;
  Bounce2::Button R_up;
  Bounce2::Button R_down;
  Bounce2::Button board_L;
  Bounce2::Button board_R;
  MPU6050 mpu6050;
  KVS kvs;
  Keys();
  void init(int conID);
  void kvs_update();
  void ShowInSerial();
  void dounnceTest();
  void bounce_update();

private:
  int _conID = 0;
  int _zero = 15;
};