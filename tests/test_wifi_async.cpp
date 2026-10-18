#include <Arduino.h>
#include "BlindsController.h"
#include "BlindsSequencer.h"
#include "SerialParser.h" 
#include "WiFiManager.h" 

// 1. Identyfikatory rolet
enum BlindId {
    BLIND_SALON_CORNER_1,
    BLIND_SALON_CORNER_2,
    BLIND_TARAS_1,
    BLIND_TARAS_2,
    BLIND_KUCHNIA,
    BLIND_MALY_POKOJ,
    BLIND_SCHODY,
    BLIND_GARAZ,
    NUM_BLINDS // Automatycznie wynosi 8
};

// 2. STRUKTURA KONFIGURACYJNA SPRZĘTU
// Definiujemy unikalny szablon opisujący parametry elektryczne i czasowe każdej rolety
struct BlindHardwareConfig {
    int pinPower;           // Pin przekaźnika zasilania
    int pinDirection;       // Pin przekaźnika kierunku
    unsigned long moveTime; // Czas pełnego ruchu w milisekundach
};

// 3. CENTRALNA TABLICA MAPOWANIA PINÓW I CZASÓW
// Wszystkie dane fizyczne szafy sterowniczej są teraz w JEDNYM miejscu.
// Kolejność wierszy odpowiada dokładnie identyfikatorom z 'enum BlindId'.
const BlindHardwareConfig BLINDS_CONFIG[NUM_BLINDS] = {
    // [ID]             = { PinPower, PinDirection, Czas w ms }
    [BLIND_SALON_CORNER_1]   = { 12,       13,           15000 },
    [BLIND_SALON_CORNER_2]   = { 14,       27,           15000 },
    [BLIND_TARAS_1]     = { 25,       26,           18000 },
    [BLIND_TARAS_2]   = { 33,       32,           12000 },
    [BLIND_KUCHNIA]   = { 19,       18,           14000 },
    [BLIND_MALY_POKOJ] = { 5,        17,           14000 },
    [BLIND_SCHODY]  = { 16,       4,            10000 },
    [BLIND_GARAZ   ]   = { 2,        15,           12000 }
};

// Globalne obiekty i sekwencer
BlindsController* blinds[NUM_BLINDS];
BlindsSequencer* sequencer = nullptr;

SerialParser* serialParser = nullptr;
WiFiManager* wifiManager = nullptr; // <-- WSKAŹNIK NA MENEDŻER WI-FI

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== INICJALIZACJA SYSTEMU SMART BLINDS ===");

    // 4. AUTOMATYCZNE TWORZENIE OBIEKTÓW W PĘTLI
    // Dzięki strukturze config, nie musimy pisać 8 linii kodu ręcznie!
    // Jeśli zmienisz piny w tablicy wyżej, ta pętla sama załaduje nowe dane.
    for (int i = 0; i < NUM_BLINDS; i++) {
        blinds[i] = new BlindsController(
            i, 
            BLINDS_CONFIG[i].pinPower, 
            BLINDS_CONFIG[i].pinDirection, 
            BLINDS_CONFIG[i].moveTime
        );
        blinds[i]->begin();
    }

    // Inicjalizacja sekwencera
    sequencer = new BlindsSequencer(blinds, NUM_BLINDS);
    serialParser = new SerialParser(sequencer); 
    Serial.println("Wszystkie rolety poprawnie zmapowane i zainicjalizowane.");

// Inicjalizacja Wi-Fi (Podajemy nazwę i hasło dla sieci ratunkowej ESP32)
    //wifiManager = new WiFiManager("SmartBlinds_Szafa", "RetroInstalacja123", sequencer);
    //wifiManager->begin(); // Spowoduje próbę zalogowania lub postawi hotspot


    // Przykładowe wywołanie
    Serial.println("\n[Scenariusz] Zamykanie wybranych stref...");
    sequencer->addToQueue(BLIND_TARAS_1, false);
    sequencer->addToQueue(BLIND_SCHODY, false);


     // =========================================================================
    // CODE BLOCK: TEST ZATRZYMANIA AWARYJNEGO (E-STOP)
    // =========================================================================
#ifdef RUN_TEST_EMERGENCY_STOP
    Serial.println("\n[URUCHOMIONO TEST]: Symulacja E-STOP...");
    sequencer->controlZoneSalonKuchnia(false); // Wrzuć strefę do kolejki
    sequencer->processQueue();                 // Rusz pierwszą roletę
    delay(1000);                               // Odczekaj sekundę ruchu
    sequencer->emergencyStopAll();             // Wywołaj alarm
#endif

    // =========================================================================
    // CODE BLOCK: TEST PROFILOWANIA / KALIBRACJI CZASU
    // =========================================================================
#ifdef RUN_TEST_CALIBRATION
    Serial.println("\n[URUCHOMIONO TEST]: Kalibracja rolety garażowej...");
    blinds[BLIND_GARAZ]->startCalibration(false); // Start w dół
    delay(14200);                                 // Symulacja 14.2 sekundy ruchu
    blinds[BLIND_GARAZ]->saveCalibration();       // Zapisz wynik we Flash
#endif

// Inicjalizacja Wi-Fi (Podajemy nazwę i hasło dla sieci ratunkowej ESP32)
    //wifiManager = new WiFiManager("SmartBlinds_Szafa", "RetroInstalacja123", sequencer);
    //wifiManager->begin();
}

void loop() {
    if (sequencer != nullptr) {
        sequencer->processQueue();
    }

      // Ciągłe, nieblokujące nasłuchiwanie na porcie szeregowym
    if (serialParser != nullptr) {
        serialParser->readSerial();
    }

    // Obsługa i monitoring tła sieciowego Wi-Fi
    //if (wifiManager != nullptr) wifiManager->handleWiFi();
}

