#pragma once
#include <Arduino.h>
#include "BlindsSequencer.h"

class SerialParser {
private:
    BlindsSequencer* sequencer; // Wskaźnik do sekwencera, by móc wywoływać jego akcje
    String inputBuffer;         // Bufor na przychodzące znaki tekstowe

public:
    // Konstruktor wiążący parser z naszym globalnym sekwencerem
    SerialParser(BlindsSequencer* seq);

    // Funkcja czytająca port szeregowy - musi być wywoływana w loop()
    void readSerial();
};
