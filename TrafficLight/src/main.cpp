#include <Arduino.h>

// Definicja pinów dla Arduino Uno (odpowiedniki D10, D11, D12 to numery 10, 11, 12)
constexpr int red_led_pin = 12;    
constexpr int yellow_led_pin = 11; 
constexpr int green_led_pin = 10;  
constexpr int pedestrain_switch_pin = 13;

// Deklaracja funkcji
void set_lights(bool red, bool yellow, bool green);

void setup() {
    // Konfiguracja pinów
    pinMode(pedestrain_switch_pin, INPUT_PULLUP); // Wewnętrzny rezystor podciągający (przycisk do GND)
    pinMode(red_led_pin, INPUT);
    pinMode(yellow_led_pin, INPUT);
    pinMode(green_led_pin, INPUT);
}

void loop() {
    // Sprawdzamy, czy przycisk został wciśnięty (stan LOW, bo zwiera do GND)
    if (digitalRead(pedestrain_switch_pin) == LOW) {
        
        // --- SEKWENCJA DLA PIESZYCH (po wciśnięciu przycisku) ---
        
        // 1. Zamiast od razu gaśnięcia, dajemy krótkie ostrzeżenie (np. żółte światło na 2 sekundy)
        set_lights(false, true, false); // żółte
        delay(2000);
        
        // 2. Czerwone dla samochodów (piesze mogą przechodzić) przez 4 sekundy
        set_lights(true, false, false); // czerwone
        delay(4000);
        
        // 3. Czerwone + żółte (przygotowanie do ruszenia) przez 1 sekundę
        set_lights(true, true, false); // czerwone + żółte
        delay(1000);
        
        // Po sekwencji pętla wróci do początku i jeśli przycisk nie jest trzymany, ustawi zielone.
        
    } else {
        
        // --- STAN DOMYŚLNY ---
        // Bez wciśnięcia przycisku: cały czas świeci się zielone
        set_lights(false, false, true); 
        
    }
}

// Funkcja sterująca diodami w trybie emulowanego open-drain:
// true -> stan niski ( LOW / Output ), świeci
// false -> stan wysokiej impedancji ( INPUT ), gaśnie
void set_lights(bool red, bool yellow, bool green) {
    // Czerwona
    if (red) {
        pinMode(red_led_pin, OUTPUT);
        digitalWrite(red_led_pin, LOW);
    } else {
        pinMode(red_led_pin, INPUT);
    }

    // Żółta
    if (yellow) {
        pinMode(yellow_led_pin, OUTPUT);
        digitalWrite(yellow_led_pin, LOW);
    } else {
        pinMode(yellow_led_pin, INPUT);
    }

    // Zielona
    if (green) {
        pinMode(green_led_pin, OUTPUT);
        digitalWrite(green_led_pin, LOW);
    } else {
        pinMode(green_led_pin, INPUT);
    }
}