#include "BlindsController.h"

#ifdef USE_NVS_PREFERENCES
#include <Preferences.h>
#endif

BlindsController::BlindsController(int blindId, int pPower, int pDirection, unsigned long pMoveTime) {
     id = blindId;
    pinPower = pPower;
    pinDirection = pDirection;
    moveTime = pMoveTime; // To staje się teraz wartością domyślną startową
    currentPosition = 0;
    exactPosition = 0.0f;
    isMoving = false;
    movingUp = false;
    lastUpdateTime = 0;
    isCalibrating = false;
    calibrationStartTime = 0;
}

void BlindsController::begin() {
    pinMode(pinPower, OUTPUT);
    pinMode(pinDirection, OUTPUT);

    digitalWrite(pinPower, HIGH);       
    digitalWrite(pinDirection, HIGH);   

#ifdef USE_NVS_PREFERENCES
    Preferences prefs;
    if (prefs.begin("blinds_state", true)) {
        char keyPos[16];
        char keyTime[16];
        snprintf(keyPos, sizeof(keyPos), "pos_%d", id);
        snprintf(keyTime, sizeof(keyTime), "time_%d", id);
        
        currentPosition = prefs.getInt(keyPos, 0);
        exactPosition = (float)currentPosition;
        
        // Dynamiczne nadpisanie czasu moveTime wartością zapisaną z udanej kalibracji.
        // Jeśli profil nie był kalibrowany, pobiera fabryczną wartość domyślną (moveTime).
        moveTime = prefs.getULong(keyTime, moveTime);
        prefs.end();
        
        Serial.printf("[Roleta %d] [NVS] Przywrócono: Pozycja=%d%%, Czas pracy=%lu ms\n", id, currentPosition, moveTime);
    }
#else
    Serial.printf("[Roleta %d] [NVS Wyłączone] Czas domyślny: %lu ms\n", id, moveTime);
#endif
}

void BlindsController::moveUp() {
    if (isMoving) stop();

    digitalWrite(pinDirection, LOW);
    delay(50); 
    digitalWrite(pinPower, LOW);
    
    // Rejestracja stanu początkowego ruchu
    isMoving = true;
    movingUp = true;
    lastUpdateTime = millis(); 
    Serial.printf("[Roleta %d] Start w GÓRĘ od pozycji: %d%%\n", id, currentPosition);
}

void BlindsController::moveDown() {
    if (isMoving) stop();

    digitalWrite(pinDirection, HIGH);
    delay(50); 
    digitalWrite(pinPower, LOW);
    
    // Rejestracja stanu początkowego ruchu
    isMoving = true;
    movingUp = false;
    lastUpdateTime = millis(); 
    Serial.printf("[Roleta %d] Start w DÓŁ od pozycji: %d%%\n", id, currentPosition);
}

void BlindsController::stop() {
    if (!isMoving) return;

    // Najpierw fizycznie odcinamy zasilanie
    digitalWrite(pinPower, HIGH); 
    delay(50);
    digitalWrite(pinDirection, HIGH);
    
    isMoving = false;

    // Wywołujemy update po raz ostatni, aby doliczyć ułamek sekundy tuż przed zatrzymaniem
    update(); 

    Serial.printf("[Roleta %d] Zatrzymano na pozycji: %d%%\n", id, currentPosition);

#ifdef USE_NVS_PREFERENCES
    Preferences prefs;
    if (prefs.begin("blinds_state", false)) {
        char key[16]; // POPRAWKA: bezpieczny bufor
        snprintf(key, sizeof(key), "pos_%d", id);
        prefs.putInt(key, currentPosition);
        prefs.end();
        Serial.printf("[Roleta %d] [NVS] Zapisano pozycję do pamięci Flash.\n", id);
    }
#endif
}

// SERCE MATEMATYCZNEGO WYLICZANIA POZYCJI
void BlindsController::update() {
    if (!isMoving) return;

    unsigned long now = millis();
    unsigned long deltaTime = now - lastUpdateTime; // Ile milisekund upłynęło od ostatniego wywołania
    lastUpdateTime = now; // Aktualizujemy punkt odniesienia

    // Obliczamy, o jaki procent zmieniła się pozycja rolety w tym ułamku czasu
    // (Używamy rzutowania na float, aby zachować precyzję dzielenia)
    float deltaPercent = ((float)deltaTime / (float)moveTime) * 100.0f;

    if (movingUp) {
        exactPosition += deltaPercent;
        if (exactPosition > 100.0f) exactPosition = 100.0f; // Ograniczenie programowe do 100%
    } else {
        exactPosition -= deltaPercent;
        if (exactPosition < 0.0f) exactPosition = 0.0f;     // Ograniczenie programowe do 0%
    }

    // Przypisujemy zaokrągloną wartość do zmiennej całkowitej, którą widzi użytkownik
    currentPosition = (int)(exactPosition + 0.5f); 
}

void BlindsController::startCalibration(bool up) {
    Serial.printf("[Roleta %d] !!! ROZPOCZĘCIE KALIBRACJI CZASU Ruchu !!!\n", id);
    isCalibrating = true;
    calibrationStartTime = millis(); // Zapisujemy moment startu stopera
    
    if (up) {
        moveUp();
    } else {
        moveDown();
    }
}

void BlindsController::saveCalibration() {
    if (!isCalibrating || !isMoving) return;

    unsigned long duration = millis() - calibrationStartTime; // Obliczamy realny czas trwania ruchu
    isCalibrating = false;
    
    // Zatrzymujemy fizyczny silnik
    stop(); 

    // Nadpisujemy stary czas nowo zmierzonym profilem
    moveTime = duration; 
    Serial.printf("[Roleta %d] !!! KALIBRACJA ZAKOŃCZONA !!! Zmierzony czas: %lu ms\n", id, moveTime);

#ifdef USE_NVS_PREFERENCES
    Preferences prefs;
    if (prefs.begin("blinds_state", false)) {
        char keyTime[16];
        snprintf(keyTime, sizeof(keyTime), "time_%d", id);
        
        // Trwale zapisujemy nowy czas w pamięci nieulotnej
        prefs.putULong(keyTime, moveTime);
        prefs.end();
        Serial.printf("[Roleta %d] [NVS] Nowy profil czasu został trwale zapisany we Flash.\n", id);
    }
#endif
}