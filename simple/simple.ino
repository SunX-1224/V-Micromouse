#define __DEBUG__

#ifdef __DEBUG__
#define pr(...) Serial.print(__VA_ARGS__)
#define pl(...) Serial.println(__VA_ARGS__)
#else
#define pr(...)
#define pl(...)
#endif

#define lm_e 6
#define lm_1 8
#define lm_2 7

#define rm_e 9
#define rm_1 11
#define rm_2 10

#define KP 20
#define KD 3
#define KI 0.0001

#define DEF_SPEED 170

enum MODE {
  S = 0, L, SL, F, SR, R, B
};

byte _STATE = S;
int _ERROR = 0, _PERROR = 0;
int PID = 0;

int ls = DEF_SPEED, rs = DEF_SPEED;

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(14 + i, INPUT);
  }

  pinMode(lm_e, OUTPUT);
  pinMode(lm_1, OUTPUT);
  pinMode(lm_2, OUTPUT);
  pinMode(rm_e, OUTPUT);
  pinMode(rm_2, OUTPUT);
  pinMode(rm_1, OUTPUT);

#ifdef __DEBUG__
  Serial.begin(9600);
#endif
  setspeed(DEF_SPEED, DEF_SPEED);
}

void loop() {
  byte value = readSensors();
  moveBot();
}

byte readSensors() {
  byte value = 0;

  for (int i = 0; i < 6; i++) {
    value = value << 1 | digitalRead(14 + i);
  }
/*
  if (value >> 5) {
    pr("L: ");
    pl(value, BIN);
    _STATE = L;
  }*/
  //if (~value & 0b11111) {
    byte fs = ~value & 0b00011111;

    _STATE = F;
  
    if (fs == 0b10000)
      _ERROR = 8;
    else if (fs == 0b11000)
      _ERROR = 5;
    else if (fs == 0b01000)
      _ERROR = 3;
    else if(fs  == 0b01100)
      _ERROR = 2;
    else if(fs == 0b00100)
      _ERROR = 0;
    else if(fs == 0b00110)
      _ERROR = -2;
    else if(fs == 0b00010)
      _ERROR = -3;
    else if(fs == 0b00011)
      _ERROR = -5;
    else if(fs == 0b00001)
      _ERROR = -8;
      

    pr("\nError : ");
    pr(_ERROR);
}

void calcPID(){
  int _TERROR = 0;
  int d = _ERROR - _PERROR;
  _PERROR = _ERROR;
  _TERROR += _ERROR;
  PID = KP * _ERROR + KD * d + KI * _TERROR;
  rs = DEF_SPEED - PID;
  ls = DEF_SPEED + PID;

  pr(" ");
  pr(ls);
  pr(" ");
  pr(rs);
  pr(" ");
  pl(PID);
}

void moveBot() {
  switch (_STATE) {
    case L:
      setspeed(DEF_SPEED, DEF_SPEED);
      setdir(0, 1, 1, 0);
      delay(200);
      break;
    case F:
      calcPID();
      setspeed(ls>255?255:ls, rs>255?255:rs);
      setdir(1, 0, 1, 0);
      break;
    case R:
      setspeed(DEF_SPEED, DEF_SPEED);
      setdir(1, 0, 0, 1);
      delay(100);
      break;
    default:
      break;
  }
}

inline void setdir(byte l1, byte l2, byte r1, byte r2) {
  digitalWrite(lm_1, l1);
  digitalWrite(lm_2, l2);
  digitalWrite(rm_1, r1);
  digitalWrite(rm_2, r2);
}

inline void setspeed(int left, int right) {
  analogWrite(lm_e, left);
  analogWrite(rm_e, right);
}
