#include "WiFiManager.h"
#include <WiFi.h>
/*
WiFiManager::WiFiManager(const char* fallbackSSID, const char* fallbackPassword, BlindsSequencer* seq) 
    : server(80) { // Inicjalizacja serwera na porcie 80
    apSSID = fallbackSSID;
    apPassword = fallbackPassword;
    connectionTimeout = 10000;
    isConnectedToHomeNetwork = false;
    sequencer = seq; // Przypisujemy sekwencer
}*/

void WiFiManager::begin() {
    // ... (Cała wcześniejsza logika logowania do Wi-Fi/AP bez zmian) ...
    // Zakładamy, że ESP32 dostał już IP. Teraz odpalamy serwer:

    // =========================================================================
    // ROZWIĄZANIE 1: SERWOWANIE STRONY WWW (DLA PRZEGLĄDARKI)
    // =========================================================================
    /*
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        // Przygotowujemy prosty kod HTML z przyciskami
        String html = "<html><head><meta charset='UTF-8'><title>Smart Blinds</title>";
        html += "<style>button { font-size: 24px; margin: 10px; padding: 15px; width: 80%; }</style></head>";
        html += "<body style='text-align: center; font-family: Arial;'>";
        html += "<h1>Sterowanie Centralne Roletami</h1>";
        html += "<button onclick=\"location.href='/api/open'\">OTWÓRZ WSZYSTKIE</button><br>";
        html += "<button onclick=\"location.href='/api/close'\">ZAMKNIJ WSZYSTKIE</button><br>";
        html += "<button style='background-color: red; color: white;' onclick=\"location.href='/api/stop'\">STOP ALL</button>";
        html += "</body></html>";
        
        request->send(200, "text/html", html); // Wysyłamy stronę jako tekst HTML
        
    });*/

    // =========================================================================
    // ROZWIĄZANIE 2: REST API (DLA APLIKACJI MOBILNEJ FLUTTER)
    // =========================================================================
    // Endpoint do otwierania wszystkich rolet
   /*
    server.on("/api/open", HTTP_GET, [this](AsyncWebServerRequest *request){
        sequencer->openAll(); // Wywołujemy akcję w kodzie ESP32
        request->send(200, "application/json", "{\"status\":\"success\",\"action\":\"opening_all\"}"); // Zwracamy JSON
    });

    // Endpoint do zamykania wszystkich rolet
    server.on("/api/close", HTTP_GET, [this](AsyncWebServerRequest *request){
        sequencer->closeAll();
        request->send(200, "application/json", "{\"status\":\"success\",\"action\":\"closing_all\"}");
    });

    // Endpoint do awaryjnego stopu
    server.on("/api/stop", HTTP_GET, [this](AsyncWebServerRequest *request){
        sequencer->emergencyStopAll();
        request->send(200, "application/json", "{\"status\":\"emergency_stop_triggered\"}");
    });

    // Uruchomienie serwera
    server.begin();
    Serial.println("[HTTP Server] Asynchroniczny serwer HTTP został uruchomiony.");
    */
}

void WiFiManager::handleWiFi() {
    // ... (Cykliczny monitoring sieci co 5 sekund bez zmian) ...
}
