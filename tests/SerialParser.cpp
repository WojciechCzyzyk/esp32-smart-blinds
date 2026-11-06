#include "SerialParser.h"

SerialParser::SerialParser(BlindsSequencer* seq) {
    sequencer = seq;
    inputBuffer = "";
}

void SerialParser::readSerial() {
    // Sprawdzamy, czy w porcie szeregowym czekają jakieś bajty do odczytania
    while (Serial.available() > 0) {
        char c = Serial.read();

        // Jeśli wykryjemy znak końca linii (Enter), czas przeanalizować całą komendę
        if (c == '\n' || c == '\r') {
            inputBuffer.trim(); // Usuwamy ukryte spacji lub białe znaki

            if (inputBuffer.length() > 0) {
                Serial.printf("[Parser] Odebrano komendę: %s\n", inputBuffer.c_str());

                // PARSOWANIE KOMEND GLOBALNYCH I STREFOWYCH
                if (inputBuffer.equalsIgnoreCase("OPEN_ALL")) {
                    sequencer->openAll();
                } 
                else if (inputBuffer.equalsIgnoreCase("CLOSE_ALL")) {
                    sequencer->closeAll();
                } 
                else if (inputBuffer.equalsIgnoreCase("STOP_ALL")) {
                    sequencer->emergencyStopAll();
                } 
                else if (inputBuffer.equalsIgnoreCase("SALON_UP")) {
                    sequencer->controlZoneSalonKuchnia(true); // true = góra
                } 
                else if (inputBuffer.equalsIgnoreCase("SALON_DOWN")) {
                    sequencer->controlZoneSalonKuchnia(false); // false = dół
                } 
                else {
                    Serial.println("[Parser] BŁĄD: Nieznana komenda! Dostępne: OPEN_ALL, CLOSE_ALL, STOP_ALL, SALON_UP, SALON_DOWN");
                }

                inputBuffer = ""; // Czyszczenie bufora na kolejną komendę
            }
        } else {
            // Jeśli to zwykły znak, dopisujemy go do końca naszego bufora tekstowego
            inputBuffer += c;
        }
    }
}
