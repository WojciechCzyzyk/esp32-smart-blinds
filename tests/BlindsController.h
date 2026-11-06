#pragma once
#include <Arduino.h>

class BlindsController {
private:
    int id;                 
    int pinPower;           
    int pinDirection;       
    int currentPosition;    
    unsigned long moveTime; // Czas pełnego ruchu (w ms) - teraz będzie ładowany dynamicznie!
    bool isMoving;          
    bool movingUp;              
    unsigned long lastUpdateTime; 
    float exactPosition;        

    // NOWE ZMIENNE DLA PODSYSTEMU KALIBRACJI:
    bool isCalibrating;          // Flaga: czy roleta jest w trybie uczenia czasów
    unsigned long calibrationStartTime; // Znacznik czasu rozpoczęcia kalibracji

public:
    BlindsController(int blindId, int pPower, int pDirection, unsigned long pMoveTime);
    void begin();
    void moveUp();
    void moveDown();
    void stop();
    void update(); 

    // FUNKCJE OBSŁUGI KALIBRACJI
    void startCalibration(bool up); // Uruchamia ruch kalibracyjny (w górę lub w dół)
    void saveCalibration();         // Zatrzymuje ruch, oblicza czas i zapisuje go we Flash

    int getId() const { return id; }
    int getPosition() const { return currentPosition; }
    bool checkIfMoving() const { return isMoving; }
    unsigned long getMoveTime() const { return moveTime; }
    void setPosition(int pos) { currentPosition = pos; exactPosition = pos; }
};


