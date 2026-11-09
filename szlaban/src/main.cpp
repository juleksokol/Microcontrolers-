#include <Arduino.h>
#include <Servo.h>
#include "./servo.hpp"


Servo myservo;
// put function declarations here:
constexpr int green_led_pin = D11;
constexpr int red_led_pin = D12;
constexpr int open_gate_button = A0;
constexpr int close_gate_button = A1;
constexpr int servo_output = D10;
unsigned int led_stat = true;
volatile unsigned long ticks;


void set_lights(bool red, bool green){
    digitalWrite(red_led_pin, red);
    digitalWrite(green_led_pin, green);
}

void servo_setup(unsigned int angle){
    myservo.write(angle);

};

void setup() {
  // put your setup code here, to run once:
  pinMode(green_led_pin, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  myservo.attach(servo_output);
  pinMode(open_gate_button, INPUT_PULLUP);
  pinMode(close_gate_button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(open_gate_button), open_gate, FALLING);
  attachInterrupt(digitalPinToInterrupt(close_gate_button), close_gate, FALLING);
  
  
}

void loop() {
  
  static unsigned long last_change = 0;
  unsigned long now1 = ticks;
   if(now1 - last_change >= 500){
     led_stat = !led_stat;
     last_change = now1;
     set_lights(led_stat, !led_stat);
   }
   action(ticks);
}

// put function definitions here:
extern "C" void HAL_SYSTICK_Callback() {
  ++ticks;
}