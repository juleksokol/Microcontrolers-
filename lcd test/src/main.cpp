#include <Arduino.h>
#include <LiquidCrystal.h>

// Przypisanie pinów zgodnie z Twoją dostępną listą: (RS, Enable, DB4, DB5, DB6, DB7)
const int rs = A1, en = A5, d4 = A6, d5 = A7, d6 = D13, d7 = D0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // Inicjalizacja ekranu 16x2
  lcd.begin(16, 2);

  // Wyświetlenie testowego napisu
  lcd.setCursor(0, 0);
  lcd.print("NIGGERS  COUNTER");

  lcd.setCursor(0, 1);
  lcd.print("==");
  lcd.setCursor(14,1);
  lcd.print("==");
}

void loop() {
  unsigned int counter = millis() / 750;
  
  // Zmienna static pamięta poprzednią sekundę, żeby odświeżać ekran tylko przy zmianie
  static unsigned int last_counter = 9999;

  if (counter != last_counter) {
    last_counter = counter;

    // Ustalamy 'i' (przesunięcie dla centrowania) w zależności od wielkości liczby:
    // i = 0 dla 1 cyfry (0-9)
    // i = 1 dla 2 cyfr (10-99)
    // i = 2 dla 3 cyfr (100-999) itd.
    int i = 0;
    if (counter >= 10 && counter < 1000) i = 1;
    else if (counter >= 1000 && counter < 100000) i = 2;
    else if (counter >= 100000 && counter < 10000000) i = 3;
    else if (counter >= 10000000) i = 4;

    // Czyszczymy linijkę spacjami, żeby poprzednie cyfry nie zostawiały "śmieci"
    lcd.setCursor(0, 1);
    lcd.print("                ");

    // Ustawiamy kursor na środku (kolumna 7 minus 'i') i wypisujemy aktualny czas
    lcd.setCursor(7 - i, 1);
    lcd.print(counter);
  }
}

 

  
