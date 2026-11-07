
🧠 Anatomia Klasy BlindsManager & BlindsSequencer (C++20)


📁 1. Plik Nagłówkowy: BlindsManager.h

cpp
#pragma once
Używaj kodu z rozwagą.
• Opis: Strażnik kompilacji. Gwarantuje, że plik nagłówkowy zostanie dołączony przez kompilator tylko raz, zapobiegając błędom wielokrotnej definicji struktur i klas.
cpp
#include <cstdint>
#include <string_view>
#include <optional>
Używaj kodu z rozwagą.
• #include <cstdint>: Wprowadza precyzyjne typy mikroprocesorowe o stałej szerokości bitowej (np. uint8_t zamiast rozmytego systemowo int).
• #include <string_view>: Standard C++20. Umożliwia operowanie na bezkosztowych, niemodyfikowalnych oknach tekstowych (referencjach do pamięci Flash), eliminując alokację pamięci RAM na kopie napisów.
• #include <optional>: Kontener bezpieczeństwa. Pozwala funkcji jawnie zwrócić informację o braku danych (std::nullopt), eliminując stosowanie niebezpiecznych wartości magicznych (np. -1 czy 0xFF).
cpp
class BlindsManager {
Używaj kodu z rozwagą.
• Opis: Definicja klasy reprezentującej logiczny mózg i maszynę stanów pojedynczej rolety.

🔒 Sekcja Prywatna (private:) – Wewnętrzny Stan Obiektu

cpp
private:
    uint8_t m_id;
    std::string_view m_name;
Używaj kodu z rozwagą.
• m_id: Unikalny identyfikator numeryczny rolety w systemie (1 bajt). Przedrostek m_ (member) oznacza zmienną składową klasy.
• m_name: Czytelna dla człowieka nazwa rolety (np. "Okno Salon"). Dane tekstowe rezydują we Flashu, manager przechowuje wyłącznie ultra-lekki wskaźnik.
cpp
    float m_currentPosition = 0.0f;
    float m_targetPosition = 0.0f;
Używaj kodu z rozwagą.
• m_currentPosition: Matematyczna, bieżąca pozycja rolety w danej milisekundzie w zakresie 0.0f (góra) do 100.0f (dół). Użycie typu float zapewnia wysoką płynność obliczeń pozycji pośrednich.
• m_targetPosition: Wirtualny punkt docelowy ruchu zadany przez użytkownika (np. żądanie otwarcia na 50%).
cpp
    float m_totalDurationMs;
    bool m_isMoving = false;
    bool m_directionUp = false;
Używaj kodu z rozwagą.
• m_totalDurationMs: Fizyczny czas pełnego przejazdu rolety od 0% do 100% wyrażony w milisekundach. Baza przeliczeniowa dla wirtualnego pozycjonowania.
• m_isMoving: Flaga logiczna sygnalizująca stan ruchu. true oznacza, że roleta wirtualnie przemieszcza się do celu.
• m_directionUp: Wektor kierunku ruchu. true – roleta zwija się w górę; false – rozwija się w dół.

🔓 Sekcja Publiczna (public:) – Interfejs Operacyjny

cpp
public:
    BlindsManager(uint8_t id, std::string_view name, float durationMs)
        : m_id(id), m_name(name), m_totalDurationMs(durationMs) {}
Używaj kodu z rozwagą.
• Konstruktor: Inicjalizuje obiekt w pamięci mikrokontrolera. Wykorzystuje listę inicjalizacyjną (po dwukropku) – najbardziej wydajny i czysty inżyniersko sposób przypisywania stanów początkowych w C++.
cpp
    void setTargetPosition(float target) {
        if (target < 0.0f) target = 0.0f;
        if (target > 100.0f) target = 100.0f;
        m_targetPosition = target;
        m_isMoving = (m_currentPosition != m_targetPosition);
        if (m_isMoving) {
            m_directionUp = (m_targetPosition < m_currentPosition);
        }
    }
Używaj kodu z rozwagą.
• setTargetPosition: Rozkaz pozycjonowania (np. funkcja "Otwórz na 50%").
	• Zapewnia ochronę graniczną danych (clamping) w zakresie 0-100%.
	• Uruchamia flagę m_isMoving, jeśli aktualne położenie różni się od zadanego.
	• Wyznacza wektor kierunku (m_directionUp) poprzez porównanie celu z pozycją bieżącą.
cpp
    void update(float deltaTimeMs) {
        if (!m_isMoving) return;
Używaj kodu z rozwagą.
• update: Serce logiki pętli czasu. Wywoływane cyklicznie w pętli głównej procesora, przyjmując jako argument precyzyjny czas deltaTimeMs, jaki upłynął od ostatniego obiegu.
	• Pierwsza linijka natychmiast przerywa wykonywanie funkcji, jeśli obiekt nie jest w stanie ruchu, drastycznie optymalizując zużycie cykli CPU.
cpp
        float step = (deltaTimeMs / m_totalDurationMs) * 100.0f;
Używaj kodu z rozwagą.
• Opis: Obliczenie kroku matematycznego. Wylicza, o jaki ułamek procenta przesunęła się roleta w czasie deltaTimeMs w relacji do czasu całkowitego przejazdu.
cpp
        if (m_directionUp) {
            m_currentPosition -= step;
            if (m_currentPosition <= m_targetPosition) {
                m_currentPosition = m_targetPosition;
                m_isMoving = false;
            }
        } else {
            m_currentPosition += step;
            if (m_currentPosition >= m_targetPosition) {
                m_currentPosition = m_targetPosition;
                m_isMoving = false;
            }
        }
    }
Używaj kodu z rozwagą.
• Opis: Aktualizacja symulacji położenia. Modyfikuje m_currentPosition o wyliczony step.
	• W przypadku zrównania lub przekroczenia progu m_targetPosition, algorytm precyzyjnie zatrzaskuje wartość na celu i gasi flagę ruchu (m_isMoving = false).
cpp
    uint8_t getId() const { return m_id; }
    std::string_view getName() const { return m_name; }
    float getCurrentPosition() const { return m_currentPosition; }
    float getTargetPosition() const { return m_targetPosition; }
    bool isMoving() const { return m_isMoving; }
Używaj kodu z rozwagą.
• Gettery: Bezpieczne funkcje dostępowe z modyfikatorem const. Umożliwiają innym modułom odczyt parametrów rolety bez ryzyka niekontrolowanej modyfikacji jej stanu wewnętrznego.
cpp
    std::optional<bool> getRequiredRelayState() const {
        if (!m_isMoving) return std::nullopt;
        return m_directionUp;
    }
};
Używaj kodu z rozwagą.
• getRequiredRelayState: Łącznik logiki ze sprzętem. Zwraca żądanie włączenia konkretnego przekaźnika (góra/dół) lub informuje o braku zapotrzebowania na zasilanie cewek (std::nullopt), gdy roleta nie wykonuje pracy.

🛠️ 2. Mechanizm Współpracy: Impuls 1s vs Wirtualna Jazda w Tle

Architektura opiera się na Separacji Obowiązków (Separation of Concerns). BlindsManager odpowiada za wirtualną matematykę pozycji %, natomiast BlindsSequencer kontroluje fizyczne przekaźniki sprzętowe i odmierza czas trwania impulsu sterującego dla magistrali Nice TTBus.

⚙️ Implementacja Pętli w BlindsSequencer.cpp

cpp
void BlindsSequencer::update(float deltaTimeMs) {
    // 1. Aktualizacja symulacji matematycznej we wszystkich roletach
    for (auto* blind : m_blinds) {
        blind->update(deltaTimeMs); 
    }

    // 2. Jeśli Sequencer jest w stanie spoczynku (State::Idle), kończymy obieg
    if (m_state == State::Idle) return;

    // 3. Odliczanie czasu fizycznego impulsu przekaźnika
    m_pulseTimerMs += deltaTimeMs;

    if (m_pulseTimerMs >= 1000.0f) {
        // --- KLUCZOWY MOMENT ARCHITEKTONICZNY ---
        this->stopPulse(); // Wyłączenie pinów GPIO (Cewki przekaźników puszczają -> 0mA)
        
        // UWAGA: Nie modyfikujemy stanu obiektów BlindsManager!
        // Ich flagi `m_isMoving` pozostają w stanie TRUE, dzięki czemu w punkcie 1
        // ich pozycje procentowe (m_currentPosition) nadal płynnie rosną w tle.
    }

    // 4. Monitoring zakończenia wirtualnego ruchu
    bool anyBlindStillMoving = false;
    for (auto* blind : m_blinds) {
        if (blind->isMoving()) {
            anyBlindStillMoving = true;
        }
    }

    // Jeśli wszystkie obiekty osiągnęły swój cel matematyczny (np. 50%):
    if (!anyBlindStillMoving) {
        m_state = State::Idle; // Pełne przejście szafy sterowniczej w stan uśpienia
        m_pulseTimerMs = 0.0f;
    }
}
Używaj kodu z rozwagą.

📊 Chronologiczny Przepływ Zdarzeń (Scenariusz: Ruch od 0% do 50%)

1. Inicjalizacja (0 ms): Wywołanie roleta.setTargetPosition(50.0f). Manager zapisuje cel, przechodzi w stan m_isMoving = true i ustala kierunek.
2. Start Sprzętu: BlindsSequencer wykrywa zapotrzebowanie na ruch, aktywuje dedykowany przekaźnik GPIO (np. KIERUNEK_DÓŁ) i zeruje swój wewnętrzny stoper m_pulseTimerMs. Fizyczny silnik rolety rusza.
3. Odcięcie Impulsu (1000 ms): Stoper m_pulseTimerMs osiąga wartość 1000 ms. Sequencer wykonuje metodę stopPulse(), zdejmując napięcie z pinów mikrokontrolera sterujących przekaźnikami. Cewki puszczają (pobór prądu spada do 0mA). Silnik rolety otrzymał impuls startowy i wykonuje pełny cykl jazdy autonomicznie.
4. Wirtualna Asynchroniczność (1001 ms – 7499 ms): Fizyczne przekaźniki pozostają rozłączone. Jednak w każdym obiegu pętli krok blind->update(deltaTimeMs) regularnie zwiększa wartość m_currentPosition (10%, 25%, 40%...).
5. Osiągnięcie Celu (7500 ms): (Zakładając pełny czas przejazdu okna równy 15 s) Matematyka w BlindsManager wylicza, że m_currentPosition osiągnęło wartość docelową 50.0f. Obiekt zatrzaskuje zmienną pozycji na wartości 50.0f i gasi flagę ruchu: m_isMoving = false.
6. Uśpienie Systemu: W tym samym obiegu BlindsSequencer wykonuje pętlę sprawdzającą. Widząc, że anyBlindStillMoving zwraca false, przełącza swój stan w State::Idle. System wirtualny idealnie zrównał się ze stanem rzeczywistym okna przy zerowym marnotrawstwie energii w stanie spoczynku.
