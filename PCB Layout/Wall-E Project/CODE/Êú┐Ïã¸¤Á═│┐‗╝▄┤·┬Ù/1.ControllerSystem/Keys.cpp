#include "Keys.h"

//MCP23017 mcp;

MCP_bounce mcp(7);

void mcp_init() {
  //mcp.begin(7);
  mcp.pinMode(MCP_PIN_L1, INPUT);
  mcp.pinMode(MCP_PIN_L2, INPUT);
  mcp.pinMode(MCP_PIN_R1, INPUT);
  mcp.pinMode(MCP_PIN_R2, INPUT);
  mcp.pinMode(MCP_PIN_UP, INPUT);
  mcp.pinMode(MCP_PIN_DOWN, INPUT);
  mcp.pinMode(MCP_PIN_LEFT, INPUT);
  mcp.pinMode(MCP_PIN_RIGHT, INPUT);
  mcp.pinMode(MCP_PIN_O, INPUT);
  mcp.pinMode(MCP_PIN_X, INPUT);
  mcp.pinMode(MCP_PIN_A, INPUT);
  mcp.pinMode(MCP_PIN_B, INPUT);
  mcp.pullUp(MCP_PIN_L1, LOW);
  mcp.pullUp(MCP_PIN_L2, LOW);
  mcp.pullUp(MCP_PIN_R1, LOW);
  mcp.pullUp(MCP_PIN_R2, LOW);
  mcp.pullUp(MCP_PIN_UP, HIGH);
  mcp.pullUp(MCP_PIN_DOWN, HIGH);
  mcp.pullUp(MCP_PIN_LEFT, HIGH);
  mcp.pullUp(MCP_PIN_RIGHT, HIGH);
  mcp.pullUp(MCP_PIN_O, HIGH);
  mcp.pullUp(MCP_PIN_X, HIGH);
  mcp.pullUp(MCP_PIN_A, HIGH);
  mcp.pullUp(MCP_PIN_B, HIGH);
}

MCP_bounce::MCP_bounce(uint8_t pin, bool default_state) {
  _pin = pin;
  _default_state = default_state;
}

bool MCP_bounce::pressed() {
  if (mcp.digitalRead(_pin) != _default_state && !_isPressed_flag) {
    delay(20);
    if (mcp.digitalRead(_pin) != _default_state) {
      _isPressed_flag = true;
      return true;
    }
  } else if (mcp.digitalRead(_pin) == _default_state) {
    _isPressed_flag = false;
  }
  return false;
}

bool MCP_bounce::read() {
  return mcp.digitalRead(_pin);
}

bool MCP_bounce::on() {
  return this->read() != _default_state;
}

bool MCP_bounce::off() {
  return this->read() == _default_state;
}

Keys::Keys()
  : mpu6050(Wire), up(MCP_PIN_UP, HIGH), down(MCP_PIN_DOWN, HIGH), left(MCP_PIN_LEFT, HIGH), right(MCP_PIN_RIGHT, HIGH), o(MCP_PIN_O, HIGH), x(MCP_PIN_X, HIGH), a(MCP_PIN_A, HIGH), b(MCP_PIN_B, HIGH), L1(MCP_PIN_L1, LOW), L2(MCP_PIN_L2, LOW), R1(MCP_PIN_R1, LOW), R2(MCP_PIN_R2, LOW) {
}

void Keys::init(int conID) {
  _conID = conID;
  Wire.setPins(41, 40);
  Wire.begin();
  mcp_init();
  mpu6050.begin();
  L_up.attach(PIN_L_UP, INPUT_PULLUP);
  L_up.interval(25);
  L_up.setPressedState(LOW);
  L_down.attach(PIN_L_DOWN, INPUT_PULLUP);
  L_down.interval(25);
  L_down.setPressedState(LOW);
  R_up.attach(PIN_R_UP, INPUT_PULLUP);
  R_up.interval(25);
  R_up.setPressedState(LOW);
  R_down.attach(PIN_R_DOWN, INPUT_PULLUP);
  R_down.interval(25);
  R_down.setPressedState(LOW);
  board_L.attach(PIN_BOARD_L, INPUT_PULLUP);
  board_L.interval(25);
  board_L.setPressedState(LOW);
  board_R.attach(PIN_BOARD_R, INPUT_PULLUP);
  board_R.interval(25);
  board_R.setPressedState(LOW);
}

void Keys::kvs_update() {
  bounce_update();
  kvs.L_up = L_up.read();
  kvs.L_down = L_down.read();
  kvs.R_up = R_up.read();
  kvs.R_down = R_down.read();
  kvs.board_L = board_L.read();
  kvs.board_R = board_R.read();
  kvs.switch_L1 = L1.read();
  kvs.switch_L2 = L2.read();
  kvs.switch_R1 = R1.read();
  kvs.switch_R2 = R2.read();
  kvs.up = up.read();
  kvs.down = down.read();
  kvs.left = left.read();
  kvs.right = right.read();
  kvs.o = o.read();
  kvs.x = x.read();
  kvs.a = a.read();
  kvs.b = b.read();
  kvs.L_knob = (int8_t)map(analogRead(PIN_L_KNOB), 4095, 0, -100, 100);
  kvs.R_knob = (int8_t)map(analogRead(PIN_R_KNOB), 4095, 0, -100, 100);
  if (_conID == 0) {
    if (analogRead(PIN_LX) < 240) {
      kvs.LX = -100;
    } else if (analogRead(PIN_LX) > 3950) {
      kvs.LX = 100;
    } else if (analogRead(PIN_LX) < 1955 - _zero) {
      kvs.LX = (int8_t)map(analogRead(PIN_LX), 240, 1955 - _zero, -100, 0);
    } else if (analogRead(PIN_LX) > 1955 + _zero) {
      kvs.LX = (int8_t)map(analogRead(PIN_LX), 1955 + _zero, 3950, 0, 100);
    } else {
      kvs.LX = 0;
    }
    if (analogRead(PIN_LY) < 240) {
      kvs.LY = -100;
    } else if (analogRead(PIN_LY) > 3850) {
      kvs.LY = 100;
    } else if (analogRead(PIN_LY) < 1952 - _zero) {
      kvs.LY = (int8_t)map(analogRead(PIN_LY), 240, 1952 - _zero, -100, 0);
    } else if (analogRead(PIN_LY) > 1952 + _zero) {
      kvs.LY = (int8_t)map(analogRead(PIN_LY), 1952 + _zero, 3850, 0, 100);
    } else {
      kvs.LY = 0;
    }
    if (analogRead(PIN_RX) > 4095) {
      kvs.RX = -100;
    } else if (analogRead(PIN_RX) < 275) {
      kvs.RX = 100;
    } else if (analogRead(PIN_RX) > 2030 + _zero) {
      kvs.RX = (int8_t)map(analogRead(PIN_RX), 4095, 2030 + _zero, -100, 0);
    } else if (analogRead(PIN_RX) < 2030 - _zero) {
      kvs.RX = (int8_t)map(analogRead(PIN_RX), 2030 - _zero, 275, 0, 100);
    } else {
      kvs.RX = 0;
    }
    if (analogRead(PIN_RY) > 3930) {
      kvs.RY = -100;
    } else if (analogRead(PIN_RY) < 320) {
      kvs.RY = 100;
    } else if (analogRead(PIN_RY) > 2025 + _zero) {
      kvs.RY = (int8_t)map(analogRead(PIN_RY), 3930, 2025 + _zero, -100, 0);
    } else if (analogRead(PIN_RY) < 2025 - _zero) {
      kvs.RY = (int8_t)map(analogRead(PIN_RY), 2025 - _zero, 320, 0, 100);
    } else {
      kvs.RY = 0;
    }
  } else {
    if (analogRead(PIN_LX) < 245) {
      kvs.LX = -100;
    } else if (analogRead(PIN_LX) > 4095) {
      kvs.LX = 100;
    } else if (analogRead(PIN_LX) < 2040 - _zero) {
      kvs.LX = (int8_t)map(analogRead(PIN_LX), 245, 2040 - _zero, -100, 0);
    } else if (analogRead(PIN_LX) > 2040 + _zero) {
      kvs.LX = (int8_t)map(analogRead(PIN_LX), 2040 + _zero, 4095, 0, 100);
    } else {
      kvs.L;
      X = 0;
    }
    if (analogRead(PIN_LY) < 370) {
      kvs.LY = -100;
    } else if (analogRead(PIN_LY) > 4075) {
      kvs.LY = 100;
    } else if (analogRead(PIN_LY) < 2160 - _zero) {
      kvs.LY = (int8_t)map(analogRead(PIN_LY), 370, 2160 - _zero, -100, 0);
    } else if (analogRead(PIN_LY) > 2160 + _zero) {
      kvs.LY = (int8_t)map(analogRead(PIN_LY), 2160 + _zero, 4075, 0, 100);
    } else {
      kvs.LY = 0;
    }
    if (analogRead(PIN_RX) > 4095) {
      kvs.RX = -100;
    } else if (analogRead(PIN_RX) < 310) {
      kvs.RX = 100;
    } else if (analogRead(PIN_RX) > 2130 + _zero) {
      kvs.RX = (int8_t)map(analogRead(PIN_RX), 4095, 2130 + _zero, -100, 0);
    } else if (analogRead(PIN_RX) < 2130 - _zero) {
      kvs.RX = (int8_t)map(analogRead(PIN_RX), 2130 - _zero, 310, 0, 100);
    } else {
      kvs.RX = 0;
    }
    if (analogRead(PIN_RY) > 3990) {
      kvs.RY = -100;
    } else if (analogRead(PIN_RY) < 190) {
      kvs.RY = 100;
    } else if (analogRead(PIN_RY) > 1993 + _zero) {
      kvs.RY = (int8_t)map(analogRead(PIN_RY), 3990, 1993 + _zero, -100, 0);
    } else if (analogRead(PIN_RY) < 1993 - _zero) {
      kvs.RY = (int8_t)map(analogRead(PIN_RY), 1993 - _zero, 190, 0, 100);
    } else {
      kvs.RY = 0;
    }
  }
  mpu6050.update();
  if (mpu6050.getAngleX() < -45) {
    kvs.angleX = -100;
  } else if (mpu6050.getAngleX() > 45) {
    kvs.angleX = 100;
  } else {
    kvs.angleX = (int8_t)map(mpu6050.getAngleX(), -45, 45, -100, 100);
  }
  if (mpu6050.getAngleY() < -45) {
    kvs.angleY = -100;
  } else if (mpu6050.getAngleY() > 45) {
    kvs.angleY = 100;
  } else {
    kvs.angleY = (int8_t)map(mpu6050.getAngleY(), -45, 45, -100, 100);
  }
}

void Keys::ShowInSerial() {
  Serial.print("LX:");
  Serial.print(kvs.LX);
  Serial.print("\tLY:");
  Serial.print(kvs.LY);
  Serial.print("\tRX:");
  Serial.print(kvs.RX);
  Serial.print("\tRY:");
  Serial.print(kvs.RY);
  Serial.print("\tKNOB:");
  Serial.print("L_knob:");
  Serial.print(kvs.L_knob);
  Serial.print(" R_knob:");
  Serial.print(kvs.R_knob);
  Serial.print(" L_UP:");
  Serial.print(kvs.L_up);
  Serial.print(" L_DOWN:");
  Serial.print(kvs.L_down);
  Serial.print(" R_UP:");
  Serial.print(kvs.R_up);
  Serial.print(" R_DOWN:");
  Serial.print(kvs.R_down);
  Serial.print("\t4_switch:");
  Serial.print(" L1:");
  Serial.print(kvs.switch_L1);
  Serial.print(" L2:");
  Serial.print(kvs.switch_L2);
  Serial.print(" R1:");
  Serial.print(kvs.switch_R1);
  Serial.print(" R2:");
  Serial.print(kvs.switch_R2);
  Serial.print("\tFUNC_KEYS:");
  Serial.print(" UP:");
  Serial.print(kvs.up);
  Serial.print(" DOWN:");
  Serial.print(kvs.down);
  Serial.print(" LEFT:");
  Serial.print(kvs.left);
  Serial.print(" RIGHT:");
  Serial.print(kvs.right);
  Serial.print(" O:");
  Serial.print(kvs.o);
  Serial.print(" X:");
  Serial.print(kvs.x);
  Serial.print(" A:");
  Serial.print(kvs.a);
  Serial.print(" B:");
  Serial.print(kvs.b);
  Serial.print("\tMPU6050 : ");
  Serial.print(" angleX: ");
  Serial.print(kvs.angleX);
  Serial.print(" angleY : ");
  Serial.println(kvs.angleY);
}

void Keys::dounnceTest() {
  while (true) {
    bounce_update();
    if (up.pressed()) { Serial.println("up pressed"); }
    if (down.pressed()) { Serial.println("down pressed"); }
    if (left.pressed()) { Serial.println("left pressed"); }
    if (right.pressed()) { Serial.println("right pressed"); }
    if (a.on()) { Serial.println("a on"); }
    if (b.on()) { Serial.println("b on"); }
    if (!o.read()) { Serial.println("o on"); }
    if (!x.read()) { Serial.println("x on"); }
    if (L1.on()) { Serial.println("L1 on"); }
    if (L2.on()) { Serial.println("L2 on"); }
    if (R1.on()) { Serial.println("R1 on"); }
    if (R2.on()) { Serial.println("R2 on"); }
    if (board_L.pressed()) { Serial.println("board_L pressed"); }
    if (board_R.pressed()) { Serial.println("board_R pressed"); }
    if (L_up.pressed()) { Serial.println("L_up pressed"); }
    if (L_down.isPressed()) { Serial.println("L_down isPressed"); }
    if (R_up.pressed()) { Serial.println("R_up pressed"); }
    if (R_down.isPressed()) { Serial.println("R_down isPressed"); }
    delay(20);
    if (left.read() == 0 && b.read() == 0) { break; }
  }
}

void Keys::bounce_update() {
  L_up.update();
  L_down.update();
  R_up.update();
  R_down.update();
  board_L.update();
  board_R.update();
}