//Uncomment the line to enable debugging mode
//#define __DEBUG

#ifdef __DEBUG
  #define debugn(...) Serial.println(__VA_ARGS__)
  #define debug(...) Serial.print(__VA_ARGS__)
#else
  #define debugn(...) 
  #define debug(...) 
#endif

#define lm_e 3
#define lm_1 4
#define lm_2 5

#define rm_e 6
#define rm_1 7
#define rm_2 8

#define DEF_SPEED 130
#define TURN_TIME 100

#define KP 5.0f
#define KD 1.4f

char _STATE = 0;
byte _ERROR =  0;
byte _PREV_ERROR = 0;
byte _PD = 0;

void setup() {
  for(int i=0; i<5; i++){
     pinMode(14+i, INPUT);
  }

  for(int i=0; i<6; i++){
    pinMode(3+i, OUTPUT);
  }

  Serial.begin(9600);
  debugn("Program Started");
}

void loop() {
  byte value = readSensors();
  
  debug("LOOP :: readings = ");
  debug(value, BIN);
  debug(" STATE = ");
  debug(_STATE);
  
  if(_STATE != 's'){
    moveBot();
  }
  
  debugn();
}

byte readSensors(){
  byte value = 0;

  for(int i=0; i<5; i++){
    value = value | (digitalRead(14 + i)<<(4-i));
  }
  
  if(value == 0b11111){
    _STATE = 's';
  }

  if(value & 0b10000){
    _STATE = 'l';
  }else if(value & 0b01110){
    _STATE = 'f';
    _ERROR = 0;
    
    if(value & 0b01000){
      _ERROR += 3;
      if(value & 0b00100)
        _ERROR -= 1;
    }
    if(value & 0b00010){
      _ERROR -= 3;
      if(value & 0b00100)
        _ERROR += 1;
    }
  }else if(value & 0b00001){
    _STATE = 'r';
  }else{
    _STATE = 'b';
  }

  return value;
}

void calculatePD(){
  byte delta = _ERROR - _PREV_ERROR;
  _PD = KP * _ERROR + KD * delta;
  _PREV_ERROR = _ERROR;
}

void moveBot(){
  if(_STATE == 'f'){
    calculatePD();
    
    debug(" PD = ");
    debug(_PD);
    
    setspeed(DEF_SPEED-_PD, DEF_SPEED+_PD);

    debug(" SPEED : l = ");
    debug(DEF_SPEED-_PD);
    debug(" r = ");
    debug(DEF_SPEED+_PD);
    
    goForward();
  } else{
    
    debug(" SPEED : l = ");
    debug(DEF_SPEED);
    debug(" r = ");
    debug(DEF_SPEED);
    
    if(_STATE == 'b'){
      setspeed(DEF_SPEED, DEF_SPEED);
      gotoTarget('l');
      _stop();
    }else{
      setspeed(DEF_SPEED, DEF_SPEED);
      gotoTarget(_STATE);
      _stop();
    }
  }
  delay(10); //small delay for resting the bot after a movement
}

void gotoTarget(char target){
  _STATE = 0;
  debug("\n   gotoTarget : ");
  int turn_time = TURN_TIME;
  do{
    if(target == 'l'){
      turnLeft();
    }else if(target == 'r'){
      turnRight();
    }
    
    // turn for a certain time and check sensors.. turn delay is reduced every time
    // to read sensors more frequently when it approaches target line
    
    delay(turn_time);
    turn_time *= 0.5;
    
    readSensors();
    
    debug("\n   readings = ");
    debug(value, BIN);
    debug(" STATE = ");
    debug(_STATE);
    
  }while(_STATE != 'f');
}

void goForward(){
  setdir(1,0,1,0);
}

void turnLeft(){
  setdir(0,1,1,0);
}

void turnRight(){
  setdir(1,0,0,1);
}

void _stop(){
  setdir(0,0,0,0);
}

inline void setdir(byte l1, byte l2, byte r1, byte r2){
	digitalWrite(lm_1, l1);
	digitalWrite(lm_2, l2);
	digitalWrite(rm_1, r1);
	digitalWrite(rm_2, r2);
}

inline void setspeed(byte left, byte right){
  analogWrite(lm_e, left);
  analogWrite(rm_e, right);
}
