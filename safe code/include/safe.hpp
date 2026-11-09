#ifndef SAFE_HPP 
#define SAFE_HPP

#include <Arduino.h>

// Definicje pinów widoczne w całym projekcie (C++17 inline constexpr)
inline constexpr int led_buildin = D13; // Wbudowana dioda LED na płytce
inline constexpr int number1 = D10;
inline constexpr int number2 = A2;
inline constexpr int number3 = D3;
inline constexpr int number4 = D4;
inline constexpr int number5 = D5;
inline constexpr int number6 = D6;
inline constexpr int number7 = A3;
inline constexpr int number8 = A4;
inline constexpr int number9 = D9;
inline constexpr int red_led_pin = D12;
inline constexpr int green_led_pin = D11;
inline constexpr int functional_button = A0;

// Deklaracje funkcji
void set_lights(bool red, bool green);
void safe_function_press();
void safe_function_release();
void safe(unsigned long now);

#endif // SAFE_HPP