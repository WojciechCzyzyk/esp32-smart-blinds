# Inteligentny Sterownik Rolet (ESP32 & Nice TTBus)

Autorski projekt inteligentnego sterownika automatyki domowej dedykowany do niezależnego zarządzania 8 roletami okiennymi (16 kanałów sterujących).

## Główne założenia projektu:
- **Mikrokontroler:** ESP32 (ESP-WROOM-32D) jako jednostka centralna.
- **Bezpieczeństwo elektryczne:** Zastosowanie mechanicznej blokady sprzętowej (podział na przekaźnik zasilania oraz kierunku), uniemożliwiającej jednoczesne podanie napięcia na kierunki góra/dół.
- **Integracja:** Sterowanie niskonapięciowymi liniami sygnałowymi (~32V DC) fabrycznej magistrali Nice TTBus.
- **Optymalizacja zasilania:** Algorytm sekwencyjnego sterowania (kolejkowanie zadań w kodzie) redukujący maksymalny pobór prądu do ~410mA, co umożliwia zasilanie całości z pojedynczego portu USB/ładowarki 5V.

## Etapy rozwoju projektu (Milestones):
- [x] Analiza instalacji elektrycznej oraz dobór komponentów sprzętowych (Moduły przekaźników 10A L/H z optoizolacją).
- [x] Opracowanie bezpiecznej mapy pinów GPIO dla ESP32.
- [ ] Konfiguracja środowiska programistycznego i testy komunikacji (Blink/UART).
- [ ] Implementacja logiki sekwencera i kolejkowania zadań.
- [ ] Testy na żywym organizmie i wdrożenie.
