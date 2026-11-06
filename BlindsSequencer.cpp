#include "BlindsSequencer.h"

BlindsSequencer::BlindsSequencer(BlindsManager** globalBlinds, int count)
    : _blindsArray(globalBlinds), _totalBlinds(count) {}

void BlindsSequencer::addToQueue(int blindId, bool directionUp) {
    if (blindId < 0 || blindId >= _totalBlinds) return;

    // Jeśli roleta już jedzie, a użytkownik zadał ruch (np. kliknął przeciwny kierunek), 
    // wywołujemy natychmiastowy stop (zgodnie z Twoją logiką z przycisków - to nie pobiera prądu ciągłego)
    if (_blindsArray[blindId]->isMoving()) {
        _blindsArray[blindId]->stop();
        return;
    }

    _taskQueue.push({blindId, directionUp});
    Serial.printf("[Sequencer] Dodano roletę ID %d do kolejki prądowej. Rozmiar: %d\n", blindId, _taskQueue.size());
}

void BlindsSequencer::processQueue() {
    // KROK 1: Stale aktualizujemy pozycję wirtualną wszystkich rolet, które aktualnie są w ruchu
    for (int i = 0; i < _totalBlinds; i++) {
        _blindsArray[i]->update();
    }

    // KROK 2: Kontrola aktywnego fizycznego impulsu przekaźnika
    if (_activePulseBlindId != -1) {
        if (millis() - _pulseStartTime >= PULSE_DURATION_MS) {
            // Minęła 1 sekunda! Odcinamy prąd cewki przekaźnika, uwalniając zasilacz USB
            _blindsArray[_activePulseBlindId]->stopPulse();
            _activePulseBlindId = -1; // Zwolnienie blokady prądowej sekwencera!
        }
        return; // Dopóki trwa sekunda impulsu, blokujemy start następnego przekaźnika
    }

    // KROK 3: Jeśli zasilacz odpoczywa i w kolejce czekają zadania, pobieramy następne
    if (!_taskQueue.empty()) {
        BlindTask nextTask = _taskQueue.front();
        _taskQueue.pop();

        _activePulseBlindId = nextTask.blindId;
        _pulseStartTime = millis();

        Serial.printf("[Sequencer] >> POBÓR PRĄDU: Start impulsu 1s dla rolety ID %d <<\n", _activePulseBlindId);
        
        if (nextTask.directionUp) {
            _blindsArray[_activePulseBlindId]->moveUp();
        } else {
            _blindsArray[_activePulseBlindId]->moveDown();
        }
    }
}

void BlindsSequencer::openAll() {
    Serial.println("[Sequencer] Wywołano: OTWÓRZ WSZYSTKIE. Kolejkowanie kaskadowe...");
    for (int i = 0; i < _totalBlinds; i++) {
        addToQueue(i, true);
    }
}

void BlindsSequencer::closeAll() {
    Serial.println("[Sequencer] Wywołano: ZAMKNIJ WSZYSTKIE. Kolejkowanie kaskadowe...");
    for (int i = 0; i < _totalBlinds; i++) {
        addToQueue(i, false);
    }
}

void BlindsSequencer::controlZoneSalonKuchnia(bool directionUp) {
    Serial.printf("[Sequencer] Strefa Dzień (Salon + Kuchnia) -> %s\n", directionUp ? "GÓRA" : "DÓŁ");
    addToQueue(ID_BLIND_SALON_CORNER_1, directionUp);
    addToQueue(ID_BLIND_SALON_CORNER_2, directionUp);
    addToQueue(ID_BLIND_TARAS_1, directionUp);
    addToQueue(ID_BLIND_TARAS_2, directionUp);
    addToQueue(ID_BLIND_KUCHNIA, directionUp);
}

void BlindsSequencer::emergencyStopAll() {
    Serial.println("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    Serial.println("[SEQUENCER] !!! ALARM: AWARYJNE ODCIĘCIE SYSTEMU !!!");
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

    // Czyszczenie kolejki FIFO za pomocą wydajnej wymiany (swap)
    std::queue<BlindTask> emptyQueue;
    std::swap(_taskQueue, emptyQueue);
    
    _activePulseBlindId = -1;

    // Bezwzględne, fizyczne i logiczne zatrzymanie każdego wyjścia w systemie
    for (int i = 0; i < _totalBlinds; i++) {
        _blindsArray[i]->stop();
    }

    Serial.println("[Sequencer] Szafa bezpieczna. Wszystkie przekaźniki rozłączone trwale.\n");
}
