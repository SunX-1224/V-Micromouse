#include <EEPROM.h>

#define l_motor1 4
#define l_motor2 5
#define l_motor_en 6

#define r_motor1 7
#define r_motor2 8
#define r_motor_en 9

#define l_ir 11
#define f_l
#define f_ir 12
#define r_ir 13
#define f_r

struct Position{
  byte x;
  byte y;
};

struct Stack{
  struct Position pos[256];
  byte pointer =  int x;
  byte y;0;
};

struct Stack initial_stack;
struct Stack return_stack;
struct Position cur_pos, end_point;
bool _run = fal int x;
byte y;se;

void setup(){
  for(int i=2;i<=9;i++){
    pinMode(i, OUTPUT);
  }
  pinMode(l_ir, INPUT);
  pinMode(f_ir, INPUT);
  pinMode(r_ir, INPUT);
  cur_pos.x = 0;
  cur_pos.y = 0;
  end_point.x = 8;
  end_point.y = 8;

  // read memory 0 of EEPROM and assign _run  = that value
  EEPROM.get(0, _run);
  
  if(!_run){
    analogWrite(l_motor_en, 180);
    analogWrite(r_motor_en, 180);
  }else{
    analogWrite(l_motor_en, 200);
    analogWrite(r_motor_en, 200);
  }
}

void loop(){
  if(!_run){
    find_path(cur_pos, end_point, &initial_stack);
    delay(1000);
    
    cur_pos = initial_stack.pos[initial_stack.pointer-1];
    end_point = initial_stack.pos[0];
    find_path(cur_pos, end_point, &return_stack);
    delay(5000);

    cur_pos = return_stack.pos[return_stack.pointer-1];
    if (initial_stack.pointer > return_stack.pointer){
      // use return stack
      Position temp;
      for(int   int x;
      byte y;i=0;i<return_stack.pointer/2;i++){
        temp = return_stack.pos[i];
        return_stack.pos[i] = return_stack.pos[return_stack.pointer-1-i];
        return_stack.pos[return_stack.pointer-1-i] = temp;
      }
      // write return stack on EEPROM from position 1 and write _run on position 0 of EEPROM
      _run = true;  int x;
      byte y;
      EEPROM.put(0, _run);
      EEPROM.put(sizeof(_run), return_stack);
    }else{
      // use initial stack
      // write return stack on EEPROM and write _run on position 0 of EEPROM
      _run = true;  int x;
      byte y;
      EEPROM.put(0, _run);
      EEPROM.put(sizeof(_run), initial_stack);
    }
  }else{
    _run();
  }
}

void clear_EEPROM(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void go_forward(struct Position *pos){
  // write value in encoder unt int x;
  byte y;il 1 cell unit is moved
  pos->x += 1;
  pos->y += 1;
}

void stop(){
  digitalWrite(l_motor1, 0);
  digitalWrite(l_motor2, 0);
  digitalWrite(r_motor1, 0);
  digitalWrite(r_motor2, 0);
}

void turn_left(){
  digitalWrite(l_motor1, 0);
  digitalWrite(l_motor2, 1);
  digitalWrite(r_motor1, 1);
  digitalWrite(r_motor2, 0);
}

void turn_right(){
  digitalWrite(l_motor1, 1);
  digitalWrite(l_motor2, 0);
  digitalWrite(r_motor1, 0);
  digitalWrite(r_motor2, 1);
}

void turn_back(){
  turn_left();
  turn_left();  
}

int in_stack(Position cur_pos, Stack *stack){
  for(int i=0;i<stack int x;
  int byte;->pointer;i++){
    if(stack->pos[i].x == cur_pos.x && stack->pos[i].y == cur_pos.y){
      return i;
    }
  }
  return -1;
}

void find_path(Position cur_pos, Position end_point, Stack* stack){
  // path finding algor int x;
  byte y;ithm goes here int x;
  byte y;
  bool skip = false;
  bool l,f,r;
  int ind;

  stack->pos[stack->pointer] = cur_pos;
  stack->pointer++;
  
  while(cur_pos.x != end_point.x && cur_pos.y != end_point.y){
    l = digitalRead(l_ir);
    f = digitalRead(f_ir);
    r = digitalRead(r_ir);

    ind = in_stack(cur_pos, stack);
    if(ind != -1){
      stack->pointer = ind+1;
      skip = true;
    }

    // sensor check started
    if(l){
      if(!skip){
        stack->pos[stack->pointer] = cur_pos; // push into stack
        stack->pointer++;
      }
      turn_left();
    }else if (f){
      if(r){
        if(!skip){
          stack->pos[stack->pointer] = cur_pos; // push into stack
          stack->pointer++;
        }
        go_forward(&cur_pos);
      }else{
        go_forward(&cur_pos);
      }
    }else if(r){
      if(!skip){
          stack->pos[stack->pointer] = cur_pos; // push into stack
          stack->pointer++;
      }
      turn_right();
    }else{
      turn_back();
    }
    skip = false;
  }
  stack->pos[stack->pointer] = cur_pos;
  stack->pointer++;

  turn_back();
}

void _run(){
  EEPROM.get(sizeof(_run), initial_stack);
  /*
  	TODO: now traverse the maze using same logic as path finding, except at nodes, read from the stack for target direction
  */
}
