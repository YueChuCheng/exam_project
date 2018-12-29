#include <Arduino.h>
#include <Ticker.h>

// Define Motor Num
#define Motor_A 0
#define Motor_B 1
// attach a LED to pPIO 2
#define LED_PIN 2 
/*#include "animate.h"*/
  bool state_move=false;
 int way=0;
//Enum and define GPIO of Motor
enum{
  Motor_A_A_Pin=25,
  Motor_A_B_Pin=33,
  Motor_B_A_Pin=27,
  Motor_B_B_Pin=26,
  };

  enum{
  right_state=1,
  left_state,
  top_state,
  down_state,
  motor_stop_state,
  };
  
  
 //define pin num in an array to use for loop

int Motor_A_Pin_num[2]={Motor_A_A_Pin,Motor_A_B_Pin};
int Motor_B_Pin_num[2]={Motor_B_A_Pin,Motor_B_B_Pin};


void refresh(){
    state_move=false;
  
  }




void top(){
  
      state_move=true;
  //motor A
    digitalWrite(Motor_A_A_Pin, HIGH);
    digitalWrite(Motor_A_B_Pin, LOW);
  //motor B
    digitalWrite(Motor_B_A_Pin, HIGH);
    digitalWrite(Motor_B_B_Pin, LOW);
    way=top_state;
  
    
    
}

void down(){
  state_move=true;
  //motor A
    digitalWrite(Motor_A_A_Pin, LOW);
    digitalWrite(Motor_A_B_Pin, HIGH);
  //motor B
    digitalWrite(Motor_B_A_Pin, LOW);
    digitalWrite(Motor_B_B_Pin, HIGH);
    way=down_state;
}

void left(){
   state_move=true;
  //motor A
    digitalWrite(Motor_A_A_Pin, HIGH);
    digitalWrite(Motor_A_B_Pin, LOW);
  //motor B
    digitalWrite(Motor_B_A_Pin, HIGH);
    digitalWrite(Motor_B_B_Pin, HIGH);
    way=left_state;
}

void right(){
   state_move=true;
  //motor A
    digitalWrite(Motor_A_A_Pin, HIGH);
    digitalWrite(Motor_A_B_Pin, HIGH);
  //motor B
    digitalWrite(Motor_B_A_Pin, HIGH);
    digitalWrite(Motor_B_B_Pin, LOW);
    way=right_state;
}

void motor_stop(){
  state_move=true;
  //motor A
    digitalWrite(Motor_A_A_Pin, HIGH);
    digitalWrite(Motor_A_B_Pin, HIGH);
  //motor B
    digitalWrite(Motor_B_A_Pin, HIGH);
    digitalWrite(Motor_B_B_Pin, HIGH);
    way=motor_stop_state;
  }
/*Ticker blinker;
Ticker toggler;
Ticker changer;
Ticker Seven_LED_Tick;
float blinkerPace = 0.1;  //seconds
const float togglePeriod = 5; //seconds
const float Seven_LED_Timer = 0.5;*/

/*void change() {
  blinkerPace = 0.5;
}

void blink() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void toggle() {
  static bool isBlinking = false;
  if (isBlinking) {
    blinker.detach();
    isBlinking = false;
  }
  else {
    blinker.attach(blinkerPace, blink);
    isBlinking = true;
  }
  digitalWrite(LED_PIN, LOW);  //make sure LED on on after toggling (pin LOW = led ON)
  }
//Show Senven Led Number function
void show_seven_Led(int number)
{
  for(int i=0;i<8;i++)
  {
    digitalWrite(Seven_Led_Pin_num[i],Seven_LED_Number[number][i]);
  }
}*/
// Initial function run once when power on
/*void setup() {*/
  /*pinMode(LED_PIN, OUTPUT);*/
  
  
 /* for(int i=0;i<8;i++) pinMode(Seven_Led_Pin_num[i],OUTPUT);
  toggler.attach(togglePeriod, toggle);
  changer.once(30, change);*/
  /*Set_Motor_Forward(Motor_A);
  Set_Motor_Forward(Motor_B);*/
/*}*/
// Loop function  when initial function over step always run loop function
/*void loop() {

}*/
