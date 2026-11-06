#include "BlindsSequencer.h"
#include <Arduino.h>

// JEŚLI MAKRO JEST WŁĄCZONE, DOŁĄCZ BIBLIOTEKĘ PREFERENCES
#ifdef USE_NVS_PREFERENCES
#include <Preferences.h>
#endif

// Sprytny trik: dołączamy nagłówek lub musimy znać wartości enumów, 
// aby sekwencer wiedział, które ID odpowiadają Salonowi i Kuchni.
// Zakładamy, że identyfikatory są zgodne z tymi w main.cpp.
 

enum BlindIdSpec {
    ID_BLIND_SALON_CORNER_1 = 0,
    ID_BLIND_SALON_CORNER_2 = 1,
    ID_BLIND_TARAS_1        = 2,
    ID_BLIND_TARAS_2        = 3,
    ID_BLIND_KUCHNIA        = 4,
    ID_NUM_BLINDS
};

BlindsSequencer::BlindsSequencer(BlindsController** globalBlinds, int count) {
    blindsArray = globalBlinds;
    totalBlinds = count;
    activeBlindId = -1; // -1 oznacza, że szafa sterownicza odpoczywa (0 prądu silników)
    motorStartTime = 0;
    currentMoveDuration = 0;
};

void BlindsSequencer::addToQueue(int blindId, bool directionUp) {
    // Zabezpieczenie przed podaniem błędnego ID rolety
    if (blindId < 0 || blindId >= totalBlinds) return;

    BlindTask newTask = {blindId, directionUp};
    taskQueue.push(newTask);
    Serial.printf("[Sequencer] Dodano roletę ID %d do kolejki. Oczekujących zadań: %d\n", blindId, taskQueue.size());
};

void BlindsSequencer::processQueue() {

  // Jeśli jakaś roleta aktualnie jedzie, stale aktualizuj jej pozycję procentową
    if (activeBlindId != -1) {
        blindsArray[activeBlindId]->update(); // <-- TA LINIA AKTUALIZUJE % W CZASIE RZECZYWISTYM

        // Sprawdzamy, czy upłynął już czas potrzebny na ruch tej rolety
        if (millis() - motorStartTime >= currentMoveDuration) {
            Serial.printf("[Sequencer] Czas minął. Zatrzymuję roletę ID %d.\n", activeBlindId);
            blindsArray[activeBlindId]->stop();
            activeBlindId = -1; 
        }
        return; 
    }

    // 1. Sprawdzenie, czy aktualnie jakaś roleta jedzie
    if (activeBlindId != -1) {
        // Sprawdzamy, czy upłynął już czas potrzebny na ruch tej rolety
        if (millis() - motorStartTime >= currentMoveDuration) {
            Serial.printf("[Sequencer] Czas minął. Zatrzymuję roletę ID %d.\n", activeBlindId);
            
            // Zatrzymujemy fizyczne przekaźniki rolety
            blindsArray[activeBlindId]->stop();
            
            activeBlindId = -1; // Zwalniamy blokadę prądową szafy
        }
        return; // Dopóki roleta jedzie, blokujemy start kolejnych!
    }

    // 2. Jeśli szafa jest wolna i w kolejce coś czeka, uruchamiamy następną roletę
    if (!taskQueue.empty()) {
        BlindTask nextTask = taskQueue.front();
        taskQueue.pop();

        activeBlindId = nextTask.blindId;
        motorStartTime = millis();
        
        // Pobieramy czas pełnego ruchu z konfiguracji danej rolety
        currentMoveDuration = blindsArray[activeBlindId]->getMoveTime();

        Serial.printf("[Sequencer] >> START sekwencji dla rolety ID %d <<\n", activeBlindId);

        // Uruchamiamy fizyczny silnik przez klasę kontrolera
        if (nextTask.directionUp) {
            blindsArray[activeBlindId]->moveUp();
        } else {
            blindsArray[activeBlindId]->moveDown();
        }
    }
}

    // Otwieranie wszystkich rolet po kolei
void BlindsSequencer::openAll() {
    Serial.println("[Sequencer] Wywołano: OTWÓRZ WSZYSTKIE ROLETY. Kolejkuję zadania...");
    for (int i = 0; i < totalBlinds; i++) {
        addToQueue(i, true); // true = góra
    }
}

// Zamykanie wszystkich rolet po kolei
void BlindsSequencer::closeAll() {
    Serial.println("[Sequencer] Wywołano: ZAMKNIJ WSZYSTKIE ROLETY. Kolejkuję zadania...");
    for (int i = 0; i < totalBlinds; i++) {
        addToQueue(i, false); // false = dół
    }
}

// Sterowanie grupowe strefą: Salon 1 + Salon 2 + Taras + Kuchnia
void BlindsSequencer::controlZoneSalonKuchnia(bool directionUp) {
    Serial.printf("[Sequencer] Wywołano kontrolę strefy SALON+KUCHNIA -> Kierunek: %s\n", 
                  directionUp ? "GÓRA" : "DÓŁ");
                  
    // Dodajemy do kolejki tylko te cztery rolety, które tworzą tę przestrzeń dzienną
    addToQueue(ID_BLIND_SALON_CORNER_1, directionUp);
    addToQueue(ID_BLIND_SALON_CORNER_2, directionUp);
    addToQueue(ID_BLIND_TARAS_1,   directionUp);
    addToQueue(ID_BLIND_TARAS_2, directionUp);
    addToQueue(ID_BLIND_KUCHNIA, directionUp);

}

void BlindsSequencer::emergencyStopAll() {
    Serial.println("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    Serial.println("[SEQUENCER] !!! ALARM: WYWOŁANO AWARYJNE ZATRZYMANIE !!!");
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

    // 1. CZYSZCZENIE KOLEJKI FIFO
    // Tworzymy pustą kolejkę i podmieniamy ją (najbardziej wydajny sposób w C++)
    std::queue<BlindTask> emptyQueue;
    std::swap(taskQueue, emptyQueue);
    
    // 2. FIZYCZNE ZATRZYMANIE AKTUALNIE PRACUJĄCEJ ROLETY
    if (activeBlindId != -1) {
        Serial.printf("[Sequencer] Natychmiastowe odcięcie zasilania rolety ID %d\n", activeBlindId);
        blindsArray[activeBlindId]->stop();
        activeBlindId = -1; // Reset blokady prądowej
    }

    // 3. PREWENCYJNE ZATRZYMANIE WSZYSTKICH ROLET W SYSTEMIE
    // Na wypadek, gdyby stany przekaźników rozjechały się z logiką programu,
    // wysyłamy sygnał stopu do absolutnie każdego wyjścia fizycznego.
    for (int i = 0; i < totalBlinds; i++) {
        blindsArray[i]->stop();
    }

    Serial.println("[Sequencer] System bezpieczny. Kolejka wyczyszczona. Przekaźniki rozłączone.\n");
}
