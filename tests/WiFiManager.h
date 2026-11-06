#pragma once
#include <Arduino.h>
//#include <ESPAsyncWebServer.h> // Dołączamy nagłówek serwera
#include "BlindsSequencer.h"   // Potrzebujemy dostępu do sekwencera

class WiFiManager {
private:
    const char* apSSID;     
    const char* apPassword; 
    unsigned long connectionTimeout; 
    bool isConnectedToHomeNetwork;

    //AsyncWebServer server;       // Obiekt serwera HTTP na porcie 80
    BlindsSequencer* sequencer;  // Wskaźnik na nasz sekwencer rolet

public:
    // Aktualizujemy konstruktor, by przyjmował też wskaźnik na sekwencer
   // WiFiManager(const char* fallbackSSID, const char* fallbackPassword, BlindsSequencer* seq);

    void begin();
    void handleWiFi();
    bool isConnected() const { return isConnectedToHomeNetwork; }
};
