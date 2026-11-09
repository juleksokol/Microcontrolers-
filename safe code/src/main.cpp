#include <Arduino.h>
#include "safe.hpp"

volatile unsigned long ticks;

void setup() {
  pinMode(number1, INPUT_PULLUP);
  pinMode(number2, INPUT_PULLUP);
  pinMode(number3, INPUT_PULLUP);
  pinMode(number4, INPUT_PULLUP);
  pinMode(number5, INPUT_PULLUP);
  pinMode(number6, INPUT_PULLUP);
  pinMode(number7, INPUT_PULLUP);
  pinMode(number8, INPUT_PULLUP);
  pinMode(number9, INPUT_PULLUP);
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(functional_button, INPUT_PULLUP);
  
}

void set_lights(bool green, bool red){
  digitalWrite(red_led_pin, red);
  digitalWrite(green_led_pin, green);
}

void loop() {
   // Przekazujemy aktualny czas (możesz użyć millis() lub ticks)
   safe(millis()); 
}

extern "C" void HALL_SYSTICK_Callback() {
  ++ticks;
}