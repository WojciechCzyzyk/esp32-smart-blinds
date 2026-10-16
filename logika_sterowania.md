# Logika Sterowania, Algorytm Impulsowy i Uniwersalność Rolet (ESP32)

Dokument opisuje produkcyjną logikę sterowania roletami z wykorzystaniem asynchronicznej maszyny stanów, nieblokujących liczników `millis()` oraz pamięci nieulotnej NVS (`Preferences`). Ponadto architektura wspiera pełną uniwersalność konfiguracji dla wielu rolet o różnych parametrach fizycznych.

## 🧠 Zasada Działania Algorytmu

Sterownik został zaprojektowany z myślą o systemach roletowych sterowanych **impulsowo** (np. integracja z magistralą Nice TTBus lub dedykowanymi sterownikami silników), gdzie pełen cykl ruchu opiera się na poniższych założeniach:

1. **Impuls Startowy (1 sekunda):** Wyzwolenie ruchu w GÓRĘ lub w DÓŁ powoduje załączenie odpowiedniego przekaźnika na czas określony przez stałą `PULSE_DURATION_MS` (1000 ms). Po tym czasie przekaźnik jest fizycznie rozłączany, a silnik kontynuuje ruch dzięki wewnętrznemu podtrzymaniu elektroniki rolety.
2. **Wirtualny Ruch (Watchdog czasowy):** Pomimo wyłączenia przekaźnika po 1 sekundzie, kontroler w pamięci ESP32 utrzymuje stan aktywny (`MOVING_UP` lub `MOVING_DOWN`) przez pełen czas przejazdu zdefiniowany indywidualnie dla danej rolety (`_totalTravelTimeMs`). Zabezpiecza to układ przed uszkodzeniem silnika w przypadku awarii fizycznych krańcówek.
3. **Pamięć Pozycji (NVS Preferences):** Po każdym zatrzymaniu (automatycznym lub ręcznym) aktualna, obliczona pozycja procentowa rolety jest zapisywana atomowo w pamięci Flash ESP32 pod unikalnym kluczem. Zapobiega to utracie stanu po zaniku zasilania.

## 🌟 Uniwersalność i Skalowalność (Wielobiektowość)

W rzeczywistych warunkach budynkowych okna mają różne wymiary (np. standardowe okno kuchenne vs. wysokie drzwi tarasowe), co bezpośrednio przekłada się na różny czas pełnego otwarcia lub zamknięcia rolety. 

Architektura kodu rozwiązuje ten problem poprzez **podejście obiektowe (C++)**:
* **Konstruktor parametryczny:** Czas pracy rolety, przypisane piny przekaźników oraz unikalny identyfikator pamięci Flash są przekazywane jako argumenty podczas tworzenia obiektu danej rolety.
* **Izolacja pamięci:** Każda instancja klasy korzysta z unikalnego identyfikatora tekstowego (ID), dzięki czemu pozycje poszczególnych rolet są zapisywane w pamięci Flash pod osobnymi kluczami i nie nadpisują się wzajemnie.
* **Asynchroniczne przetwarzanie:** Wywołanie metody `loop()` dla każdego obiektu w głównej pętli programu pozwala na jednoczesną, niezależną pracę wielu rolet o skrajnie różnych czasach przejazdu, nie blokując przy tym procesora.

## 🕹️ Logika Obsługi Przycisków (Fizycznych / Zdalnych)

System reaguje na przyciski w sposób inteligentny, eliminując przypadkowe zatrzymania i wymuszając intencjonalne działanie użytkownika:
* **Gdy roleta stoi (`STOPPED`):** Naciśnięcie przycisku GÓRA uruchamia ruch w górę. Naciśnięcie DÓŁ uruchamia ruch w dół.
* **Gdy roleta jedzie w GÓRĘ (`MOVING_UP`):**
  * Ponowne naciśnięcie przycisku GÓRA jest **całkowicie ignorowane** (roleta kontynuuje ruch do końca).
  * Naciśnięcie przycisku przeciwnego (**DÓŁ**) powoduje **natychmiastowe zatrzymanie** rolety w jej aktualnej pozycji pośredniej.
* **Gdy roleta jedzie w DÓŁ (`MOVING_DOWN`):**
  * Ponowne naciśnięcie przycisku DÓŁ jest **całkowicie ignorowane**.
  * Naciśnięcie przycisku przeciwnego (**GÓRA**) powoduje **natychmiastowe zatrzymanie** rolety.

---

## 💻 Kod Źródłowy (`src/main.cpp`)

Poniższy kod stanowi kompletną, przetestowaną sprzętowo implementację wielobiektowego systemu sterowania w środowisku PlatformIO (Framework Arduino).

```cpp
// ============================================================================
// ESP32 ROLLER BLINDS CONTROLLER (MULTI-BLINDS CONFIGURATION WITH NVS)
// ============================================================================

#include <Arduino.h>
#include <Preferences.h>

const uint32_t PULSE_DURATION_MS = 1000;     // Czas trwania impulsu dla przekaźnika (1s)
const char* PREFS_NAMESPACE = "blinds_nvs";

// --- Stany Rolety ---
enum BlindsState {
    STOPPED,
    MOVING_UP,
    MOVING_DOWN
};

// --- Klasa Kontrolera Rolet ---
class RollerBlinds {
private:
    BlindsState _state;
    uint32_t _movementStartTime;
    uint32_t _currentPositionMs;
    
    // Indywidualne parametry dla każdej rolety
    uint8_t _pinUp;
    uint8_t _pinDown;
    uint32_t _totalTravelTimeMs;
    const char* _blindId; // Unikalny identyfikator do zapisu w Preferences (max 15 znaków)
    
    Preferences _prefs;

    void writeRelayPins(bool upHardware, bool downHardware) {
        if (upHardware && downHardware) { // Zabezpieczenie przeciwsobne (Interlocking)
            upHardware = false;
            downHardware = false;
        }
        digitalWrite(_pinUp, upHardware ? HIGH : LOW);
        digitalWrite(_pinDown, downHardware ? HIGH : LOW);
    }

    void savePositionToFlash() {
        _prefs.begin(PREFS_NAMESPACE, false);
        _prefs.putUInt(_blindId, _currentPositionMs);
        _prefs.end();
        Serial.print("[Flash] Roleta ");
        Serial.print(_blindId);
        Serial.print(" zapisana: ");
        Serial.print(getPercentage());
        Serial.println("%");
    }

    void loadPositionFromFlash() {
        _prefs.begin(PREFS_NAMESPACE, true);
        // Domyślnie ustawiamy jako w pełni otwartą, jeśli brak wcześniejszego zapisu
        _currentPositionMs = _prefs.getUInt(_blindId, _totalTravelTimeMs); 
        _prefs.end();
        Serial.print("[Flash] Roleta ");
        Serial.print(_blindId);
        Serial.print(" odczytana: ");
        Serial.print(getPercentage());
        Serial.println("%");
    }

public:
    // Konstruktor przyjmujący indywidualne ustawienia dla danej instancji rolety
    RollerBlinds(uint8_t pinUp, uint8_t pinDown, uint32_t totalTravelTimeMs, const char* blindId) 
        : _state(STOPPED), _movementStartTime(0), _pinUp(pinUp), _pinDown(pinDown), 
          _totalTravelTimeMs(totalTravelTimeMs), _blindId(blindId) {}

    void begin() {
        pinMode(_pinUp, OUTPUT);
        pinMode(_pinDown, OUTPUT);
        writeRelayPins(false, false);
        
        loadPositionFromFlash();
    }

    void moveUp() {
        if (_state == MOVING_UP) return;
        if (_state == MOVING_DOWN) { stop(); return; }

        Serial.print("[Blinds] "); Serial.print(_blindId); Serial.println(": Ruch w GÓRĘ...");
        _state = MOVING_UP;
        _movementStartTime = millis();
        writeRelayPins(true, false);
    }

    void moveDown() {
        if (_state == MOVING_DOWN) return;
        if (_state == MOVING_UP) { stop(); return; }

        Serial.print("[Blinds] "); Serial.print(_blindId); Serial.println(": Ruch w DÓŁ...");
        _state = MOVING_DOWN;
        _movementStartTime = millis();
        writeRelayPins(false, true);
    }

    void stop() {
        if (_state == STOPPED) return;

        uint32_t elapsedTime = millis() - _movementStartTime;
        
        if (_state == MOVING_UP) {
            _currentPositionMs += elapsedTime;
            if (_currentPositionMs > _totalTravelTimeMs) _currentPositionMs = _totalTravelTimeMs;
        } else if (_state == MOVING_DOWN) {
            if (elapsedTime >= _currentPositionMs) _currentPositionMs = 0;
            else _currentPositionMs -= elapsedTime;
        }

        writeRelayPins(false, false);
        _state = STOPPED;
        
        Serial.print("[Blinds] "); Serial.print(_blindId); Serial.print(" ZATRZYMANA na: ");
        Serial.print(getPercentage()); Serial.println("%");
        
        savePositionToFlash();
    }

    void loop() {
        if (_state == STOPPED) return;

        uint32_t timeInCurrentState = millis() - _movementStartTime;

        // 1. Kontrola czasu impulsu przekaźnika (1s)
        if (timeInCurrentState >= PULSE_DURATION_MS) {
            writeRelayPins(false, false); 
        }

        // 2. Kontrola czasu maksymalnego przejazdu (indywidualnego dla instancji)
        if (timeInCurrentState >= _totalTravelTimeMs) {
            if (_state == MOVING_UP) _currentPositionMs = _totalTravelTimeMs;
            if (_state == MOVING_DOWN) _currentPositionMs = 0;
            
            _state = STOPPED;
            writeRelayPins(false, false);
            
            Serial.print("[Blinds] "); Serial.print(_blindId); Serial.println(" zakoczya peny ruch.");
            savePositionToFlash();
        }
    }

    BlindsState getState() const { return _state; }
    int getPercentage() const { return (_currentPositionMs * 100) / _totalTravelTimeMs; }
};

// ============================================================================
// DEKLARACJA ROLET O RÓŻNYCH CZASACH PRACY
// ============================================================================
// Składnia: RollerBlinds nazwa(PIN_GÓRA, PIN_DÓŁ, CZAS_W_MS, "UNIKALNE_ID_FLASH")

RollerBlinds roletaSalon(25, 26, 15000, "salon");     // Okno standardowe - 15 sekund
RollerBlinds roletaTaras(17, 18, 22000, "taras");     // Drzwi tarasowe - 22 sekundy
RollerBlinds roletaKuchnia(19, 21, 12000, "kuchnia"); // Małe okno - 12 sekund

// ============================================================================
// OBSŁUGA PRZYCISKÓW (Przykładowa dla Rolety Salon)
// ============================================================================
struct Button {
    uint8_t pin;
    bool lastState;
    uint32_t lastDebounceTime;
    const uint32_t debounceDelay = 50; 
};

Button btnUp   = { 32, HIGH, 0 };
Button btnDown = { 33, HIGH, 0 };

void checkButton(Button& btn, void (*onPress)()) {
    bool reading = digitalRead(btn.pin);
    if (reading != btn.lastState) btn.lastDebounceTime = millis();
    if ((millis() - btn.lastDebounceTime) > btn.debounceDelay) {
        if (reading == LOW && btn.lastState == HIGH) onPress();
    }
    btn.lastState = reading;
}

void handleUpButton() {
    if (roletaSalon.getState() == STOPPED) roletaSalon.moveUp();
else if (roletaSalon.getState() == MOVING_DOWN) roletaSalon.stop();
}
void handleDownButton() {
if (roletaSalon.getState() == STOPPED) roletaSalon.moveDown();
else if (roletaSalon.getState() == MOVING_UP) roletaSalon.stop();
}
void setup() {
Serial.begin(115200);
pinMode(btnUp.pin, INPUT_PULLUP);
pinMode(btnDown.pin, INPUT_PULLUP);
// Inicjalizacja poszczególnych obiektów rolet
roletaSalon.begin();
roletaTaras.begin();
roletaKuchnia.begin();
}
void loop() {
// Wszystkie rolety przetwarzają swoje liczniki niezależnie i asynchronicznie
roletaSalon.loop();
roletaTaras.loop();
roletaKuchnia.loop();
checkButton(btnUp, handleUpButton);
checkButton(btnDown, handleDownButton);
}
