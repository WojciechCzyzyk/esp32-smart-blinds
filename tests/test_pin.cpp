#include <Arduino.h>

// Używamy pinu RELAY_UP_PIN (25) z docelowego projektu do testu migania
const int TEST_PIN = 25; 

void setup() {
    // Inicjalizacja portu szeregowego
    Serial.begin(115200);
    delay(1000); // Krótka pauza na stabilizację portu
    
    Serial.println("=========================================");
    Serial.println("   ESP32 PLATFORMIO TEST ROZPOCZĘTY!    ");
    Serial.println("=========================================");
    
    // Ustawienie pinu jako wyjście
    pinMode(TEST_PIN, OUTPUT);
}

void loop() {
    Serial.println("[TEST] Pin 25 -> STAN WYSOKI (HIGH)");
    digitalWrite(TEST_PIN, HIGH);
    delay(2000);

    Serial.println("[TEST] Pin 25 -> STAN NISKI (LOW)");
    digitalWrite(TEST_PIN, LOW);
    delay(2000);
}
