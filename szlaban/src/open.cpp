#include <Arduino.h>
#include <Servo.h>
#include "./servo.hpp"







enum class states {opened, opening, closed, closing};

volatile states current = states::closed;

unsigned long start_state = 0;

volatile unsigned long current_time = 0;

unsigned long opening_time = 5000;

unsigned long waiting_time = 1000;

unsigned long state_start = 0;


void open_gate(){
    if(current == states::closed){
        current_time = micros();
        current = states::opening;
       
    }

}

void close_gate(){
    if(current == states::opened){
        current_time = micros();
        current = states::closing;
    }
}


void change_state (states next, unsigned long now){
    current = next;
    state_start = now;

}
void action(unsigned long now){
    switch(current){
        case states::closed: {
            set_lights(true, false);
            servo_setup(0); // Upewniamy się, że na pewno jest na 0°
        } break;
        
        case states::opening:{ 
            set_lights(true, true);
            
            // Obliczamy, ile czasu minęło od rozpoczęcia otwierania
            unsigned long elapsed = now - state_start;
            
            if (elapsed < opening_time) {
                // Płynnie mapujemy czas na kąt od 0 do 90 stopni w zadanym czasie opening_time
                int current_angle = map(elapsed, 0, opening_time, 0, 90);
                servo_setup(current_angle);
            } else {
                // Gdy czas minął, upewniamy się, że serwo jest na 90° i przechodzimy dalej
                servo_setup(90);
                change_state(states::opened, now);  
            }
        } break;
        
        case states::opened:{
            set_lights(false, true);
            servo_setup(90); // Upewniamy się, że jest na 90°
        } break;
        
        case states::closing:{
            set_lights(true, true);
            
            // Obliczamy, ile czasu minęło od rozpoczęcia zamykania
            unsigned long elapsed = now - state_start;
            
            if (elapsed < opening_time) {
                // Płynnie mapujemy czas na kąt od 90 do 0 stopni
                int current_angle = map(elapsed, 0, opening_time, 90, 0);
                servo_setup(current_angle);
            } else {
                // Gdy czas minął, upewniamy się, że serwo jest na 0° i zamykamy stan
                servo_setup(0);
                change_state(states::closed, now);  
            }
        } break;
    }
}




