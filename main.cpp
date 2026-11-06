// ============================================================================
// ESP32 SMART BLINDS SYSTEM — CENTRAL PRODUCTION CONTROLLER (C++20)
// ============================================================================

#include <Arduino.h>
#include "BlindsManager.h"
#include "BlindsSequencer.h"

// 1. Definicja fizycznej mapy pinów dla szafy sterowniczej (Zgodna z ESP32 GPIO)
// Para przekaźników dla każdej rolety: Pin Zasilania (Power) oraz Pin Kierunku (Direction)
namespace HardwareConfig {
    constexpr uint8_t CORNER_1_PWR = 25; constexpr uint8_t CORNER_1_DIR = 26;
    constexpr uint8_t CORNER_2_PWR = 27; constexpr uint8_t CORNER_2_DIR = 28;
    constexpr uint8_t TARAS_1_PWR  = 12; constexpr uint8_t TARAS_1_DIR  = 13;
    constexpr uint8_t TARAS_2_PWR  = 14; constexpr uint8_t TARAS_2_DIR  = 15;
    constexpr uint8_t KUCHNIA_PWR  = 32; constexpr uint8_t KUCHNIA_DIR  = 33;
}

// 2. Alokacja obiektów rolet z fabrycznymi (domyślnymi) czasami pełnego przejazdu
// Wartości te zostaną automatycznie nadpisane z pamięci Flash (NVS) po udanej kalibracji.
BlindsManager blindSalonCorner1(ID_BLIND_SALON_CORNER_1, HardwareConfig::CORNER_1_PWR, HardwareConfig::CORNER_1_DIR, 18000); // 18s
BlindsManager blindSalonCorner2(ID_BLIND_SALON_CORNER_2, HardwareConfig::CORNER_2_PWR, HardwareConfig::CORNER_2_DIR, 18000); // 18s
BlindsManager blindTaras1(ID_BLIND_TARAS_1, HardwareConfig::TARAS_1_PWR, HardwareConfig::TARAS_1_DIR, 22000);               // 22s
BlindsManager blindTaras2(ID_BLIND_TARAS_2, HardwareConfig::TARAS_2_PWR, HardwareConfig::TARAS_2_DIR, 22000);               // 22s
BlindsManager blindKuchnia(ID_BLIND_KUCHNIA, HardwareConfig::KUCHNIA_PWR, HardwareConfig::KUCHNIA_DIR, 15000);               // 15s

// 3. Centralna tablica wskaźników — interfejs dla orkiestratora sekwencyjnego
BlindsManager* globalBlinds[ID_NUM_BLINDS] = {
    &blindSalonCorner1,
    &blindSalonCorner2,
    &blindTaras1,
    &blindTaras2,
    &blindKuchnia
};

// 4. Instancja sekwencera prądowego zarządzającego budżetem USB
BlindsSequencer sequencer(globalBlinds, ID_NUM_BLINDS);

void setup() {
    // Inicjalizacja portu diagnostycznego
    Serial.begin(115200);
    Serial.println("\n--- [System] Inicjalizacja Sterownika Rolet ESP32 (C++20) ---");

    // Sprzętowy start każdego managera (ustawienie pinMode, wymuszenie HIGH na przekaźnikach i odczyt NVS)
    for (int i = 0; i < ID_NUM_BLINDS; i++) {
        globalBlinds[i]->begin();
    }

    Serial.println("[System] Szafa bezpieczna. Budżet prądowy USB zabezpieczony.");
    Serial.println("[System] Oczekiwanie na komendy (Kolejka FIFO gotowa)...\n");
}

void loop() {
    // Sercem systemu jest ciągłe, nieblokujące odpytywanie maszyn stanów.
    // Metoda processQueue() dba o impuls 1s, rozłącza przekaźniki i aktualizuje pozycję % w tle.
    sequencer.processQueue();

    // Tutaj w przyszłości wejdzie obsługa sieciowa / asynchroniczny serwer HTTP i WebSockets.
    // Przykładowe wywołania sterowania z poziomu aplikacji Flutter lub przycisków fizycznych:
    //
    // 1. Ruch pojedynczego okna (bezpiecznie przez kolejkę prądową):
    //    sequencer.addToQueue(ID_BLIND_KUCHNIA, true); // Kuchnia w górę
    //
    // 2. Globalne otwarcie domu (Kaskada 8 sekund dla przekaźników):
    //    sequencer.openAll();
    //
    // 3. Sterowanie dedykowaną strefą dzienną:
    //    sequencer.controlZoneSalonKuchnia(false); // Zamknij przestrzeń wspólną
    //
    // 4. Awaryjne, natychmiastowe zatrzymanie zrzucające zasilanie z cewek:
    //    sequencer.emergencyStopAll();
}
