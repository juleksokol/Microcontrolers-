#include "./safe.hpp"

enum class states {locked, unlocking, unlocked, locking, change_code};

volatile states current = states::locked;

// Zmienne do obsługi zmiennej długości kodu
constexpr int max_code_length = 9;
int code_length = 4; // Początkowa długość kodu
int safe_code[max_code_length] = {1, 2, 3, 4, 0, 0, 0, 0, 0}; // Bufor na stary kod

int entered_code[max_code_length];
int new_entered_code[max_code_length];

int buttons_tab[9] = {number1, number2, number3, number4, number5, number6, number7, number8, number9};
bool last_buttons_state[9] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

int current_index = 0;
int led_red_blinks_counter = 0;

unsigned long last_led_change_time = 0;
bool current_led_state = LOW;

// Zmienne do pomiaru czasu przytrzymania przycisku funkcyjnego
unsigned long func_btn_press_start = 0;
bool func_btn_was_pressed = false;

// Pomocnicza flaga dla trybu zmiany kodu (0 = wybieramy długość, 1 = wpisujemy nowy kod)
int change_code_substate = 0; 
int new_target_length = 0;

void safe(unsigned long now){
    
    // --- OBSŁUGA PRZYCISKU FUNKCYJNEGO (Przytrzymanie 5s = zmiana kodu) ---
    int func_btn_state = digitalRead(functional_button);
    
    if (func_btn_state == LOW) {
        if (!func_btn_was_pressed) {
            func_btn_press_start = now;
            func_btn_was_pressed = true;
        } else {
            if (current == states::locked && (now - func_btn_press_start >= 5000)) {
                current = states::change_code;
                change_code_substate = 0; // Krok 1: Wybór nowej długości
                current_index = 0;
                last_led_change_time = now;
                
                digitalWrite(LED_BUILTIN, HIGH);
                delay(200);
                digitalWrite(LED_BUILTIN, LOW);
                func_btn_was_pressed = false;
            }
        }
    } else {
        if (func_btn_was_pressed) {
            unsigned long press_duration = now - func_btn_press_start;
            if (press_duration < 5000 && press_duration > 50) {
                if (current == states::locked) {
                    current = states::unlocking;
                    current_index = 0;
                    last_led_change_time = now;
                } else if (current == states::unlocked) {
                    current = states::locking;
                    led_red_blinks_counter = 0;
                    last_led_change_time = now;
                }
            }
            func_btn_was_pressed = false;
        }
    }

    // --- MASZYNA STANÓW SEJFU ---
    switch(current){
        case states::locked: {
            set_lights(false, true); 
        } break; 
        
        case states::unlocked: {
            set_lights(true, false); 
        } break;

        case states::unlocking: {
            if(now - last_led_change_time >= 500){
                current_led_state = !current_led_state;
                last_led_change_time = now;
                set_lights(current_led_state, !current_led_state);
            }
            
            // Skanowanie 9 przycisków dla aktualnej (zmiennej) długości kodu
            for(int i = 0; i < 9; i++){
                int button_state = digitalRead(buttons_tab[i]);
                
                if(button_state == LOW && last_buttons_state[i] == HIGH){
                    int pressed_button = i + 1; 
                    entered_code[current_index] = pressed_button;
                    current_index++;
                    
                    digitalWrite(LED_BUILTIN, HIGH);
                    delay(50);
                    digitalWrite(LED_BUILTIN, LOW);

                    if(current_index >= code_length){
                        bool code_matches = true;
                        for(int j = 0; j < code_length; j++){
                            if(entered_code[j] != safe_code[j]){
                                code_matches = false;
                                break;
                            }
                        }
                        
                        if(code_matches){
                            current = states::unlocked;
                        } else {
                            current = states::locking; 
                            led_red_blinks_counter = 0;
                            last_led_change_time = now;
                        }
                    }
                }
                last_buttons_state[i] = button_state;
            }
        } break;
        
        case states::change_code: {
            if(now - last_led_change_time >= 200){
                current_led_state = !current_led_state;
                last_led_change_time = now;
                set_lights(current_led_state, current_led_state); 
            }

            for(int i = 0; i < 9; i++){
                int button_state = digitalRead(buttons_tab[i]);
                
                if(button_state == LOW && last_buttons_state[i] == HIGH){
                    int pressed_button = i + 1; 
                    
                    digitalWrite(LED_BUILTIN, HIGH);
                    delay(50);
                    digitalWrite(LED_BUILTIN, LOW);

                    if (change_code_substate == 0) {
                        // KROK 1: Pierwszy wciśnięty przycisk określa nową długość kodu (np. od 1 do 9)
                        new_target_length = pressed_button;
                        if (new_target_length > max_code_length) new_target_length = max_code_length; // zabezpieczenie
                        
                        change_code_substate = 1; // Przechodzimy do wpisywania cyfr nowego kodu
                        current_index = 0;
                    } 
                    else if (change_code_substate == 1) {
                        // KROK 2: Wpisywanie kolejnych cyfr nowego kodu
                        new_entered_code[current_index] = pressed_button;
                        current_index++;

                        // Gdy wpisano tyle cyfr, ile wynosiła zadeklarowana długość
                        if (current_index >= new_target_length) {
                            code_length = new_target_length; // Aktualizujemy globalną długość kodu
                            for(int j = 0; j < code_length; j++){
                                safe_code[j] = new_entered_code[j]; // Zapisujemy nowy kod
                            }
                            current = states::locked; // Powrót do zamknięcia
                        }
                    }
                }
                last_buttons_state[i] = button_state;
            }
        } break;

        case states::locking: {
            if(led_red_blinks_counter < 6){ 
                if(now - last_led_change_time >= 250){
                    current_led_state = !current_led_state;
                    last_led_change_time = now;
                    set_lights(LOW, current_led_state); 
                    led_red_blinks_counter++;
                }
            } else {
                current = states::locked; 
            }
        } break;
        
        default: break;
    }
}