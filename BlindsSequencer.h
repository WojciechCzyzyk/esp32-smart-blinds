#pragma once

#include <Arduino.h>
#include "BlindsManager.h"
#include <queue>

enum BlindIdSpec : uint8_t {
    ID_BLIND_SALON_CORNER_1 = 0,
    ID_BLIND_SALON_CORNER_2 = 1,
    ID_BLIND_TARAS_1        = 2,
    ID_BLIND_TARAS_2        = 3,
    ID_BLIND_KUCHNIA        = 4,
    ID_NUM_BLINDS
};

struct BlindTask {
    int blindId;
    bool directionUp;
};

class BlindsSequencer {
private:
    BlindsManager** _blindsArray;
    int _totalBlinds;
    
    int _activePulseBlindId{-1}; // ID rolety, która aktualnie pobiera prąd impulsu
    uint32_t _pulseStartTime{0};
    
    std::queue<BlindTask> _taskQueue;
    static constexpr uint32_t PULSE_DURATION_MS = 1000; // Żelazna 1 sekunda ochrony USB

public:
    BlindsSequencer(BlindsManager** globalBlinds, int count);

    void addToQueue(int blindId, bool directionUp);
    void processQueue(); // Główna maszyna stanów sekwencera w loop()

    // Funkcje sterowania grupowego i strefowego z Twojego projektu
    void openAll();
    void closeAll();
    void controlZoneSalonKuchnia(bool directionUp);
    void emergencyStopAll();
};
