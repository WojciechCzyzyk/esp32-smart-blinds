#pragma once
#include <queue>
#include "BlindsController.h"

// Struktura zadania w kolejce
struct BlindTask {
    int blindId;      // ID rolety (0-7)
    bool directionUp; // true = góra, false = dół
};

class BlindsSequencer {
private:
    std::queue<BlindTask> taskQueue; // Kolejka FIFO
    BlindsController** blindsArray;  // Wskaźnik na tablicę naszych 8 rolet
    int totalBlinds;                 // Całkowita liczba rolet w systemie (8)
    
    int activeBlindId;               // ID aktualnie poruszającej się rolety (-1 jeśli brak)
    unsigned long motorStartTime;    // Czas startu aktualnego ruchu
    unsigned long currentMoveDuration; // Ile czasu ma się ruszać ta konkretna roleta

public:
    // Konstruktor przyjmujący tablicę rolet i ich ilość
    BlindsSequencer(BlindsController** globalBlinds, int count);
    
    // Dodawanie żądania do kolejki
    void addToQueue(int blindId, bool directionUp);
    
    // Serce systemu - przetwarzanie kolejki (wywoływane w loop)
    void processQueue();
    
    int getQueueLength() const { return taskQueue.size(); }

    // NOWE FUNKCJE OBSŁUGI STREF I GLOBALNYCH POLECEŃ
    void openAll();                    // Otwiera wszystkie 8 rolet
    void closeAll();                   // Zamyka wszystkie 8 rolet
    void controlZoneSalonKuchnia(bool directionUp); // Steruje wspólnie strefą Salon + Kuchnia
     // NOWA FUNKCJA: AWARYJNE ZATRZYMANIE SYSTEMU
    void emergencyStopAll();
};

