//pseudo code for line micromouse
// TODO : implement remaining code according to the examples, suggestions, etc

//pin declarations
#define l_pin 2
#define f_pin 3
#define r_pin 4


// data structure 
struct Stack{
  char arr[256];
  int top;
};

//variable declaration
int l, r, f; //sensor readings
struct Stack st1, st2; // two stacks for front and reverse path tracing
bool solve_mode = true;

void setup(){
  st1.top=0;    //init stack top to 0
  st2.top=0;
  pinMode(l_pin, INPUT); // set pinmode for ir sensors to input mode
  pinMode(f_pin, INPUT);
  pinMode(r_pin, INPUT);
}

void loop(){
  if(solve_mode){
    find_path(&st1); // your job is to implement backward tracing of path and compare between the paths (optional)
    go_back();
    find_path(&st2);
    //c
  }else{
    _run();
  }
}

void find_path(struct Stack *stk){
  sensor_read();

  if(l==1){
    go_left();
    //push 'l' into the stack
    check_nodes(stk); 
  }
  else if(f==1){
    go_forward();
    if(r==1){     // current position is a node if there is at least one turn.. in this case 'r'..else it just a line not a node
      //push 'f' into the stack
      check_nodes(stk);
    }
  }else if(r==1){
    go_right();
    //push 'r' into the stack
    check_nodes(stk);
  }else{
    go_back(); // no need to check nodes if turn is back..
    //push 'b' into the stack
  }
}

void check_nodes(struct Stack *stk){
  char s1, s3;
  if(stk->arr[stk->top-2] == 'b'){
    s3 = //pop the top data from stack and store in s3
    //pop the next data i.e. 'b'.. no need to store it anywhere
    s1 = //pop the next data of the stack and store in s1

    // check the values of s1 and s3 and push the respective direction (l,f,r,b) in the stack 
    //example
    if(s1 =='l' && s3=='l'){    // if bot turned left, returned back and turned left again, then the bot have to go forward in that node
      //push 'f' into the stack
    }else if(s=='l' && s3 =='r'){ // if bot turned left, returned back and turned right, then the bot have to turn back in that node
      //push 'b' into the stack
    }
    // implement remaining conditions
  }
}

void sensor_read(){
  l = digitalRead(l_pin);
  f = digitalRead(f_pin);
  r = digitalRead(r_pin);
}


void go_left(){
  //loop this routine
    //turn left motor back and right motor forward 
    //delay for 10 milliseconds
    //stop both motors
    //check if front sensor detected line and other sensor detected white space..i.e. f==1 && r==0 && l==0
    //if true then bot has taken full left turn so break from loop
    //else continue
}

void go_forward(){
  // turn both motor forward and use PID to adjust position if extra sensors are available in front direction..(there are no extra sensors for PID in this pseudo code)
}

void go_right(){
  //loop this routine
    //turn right motor back and left motor forward 
    //delay for 10 milliseconds
    //stop both motors
    //check if front sensor detected line and other sensor detected white space..i.e. f==1 && r==0 && l==0
    //if true then bot has taken full right turn so break from loop
    //else continue
}

void go_back(){
  go_left();  //same as turning left
}
