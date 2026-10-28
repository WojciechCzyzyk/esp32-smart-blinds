# Portfolio Kompetencji i Współpracy z AI (AI-Assisted Engineering)

Ten plik stanowi rejestr zaawansowanych kompetencji inżynierskich, architektonicznych oraz nowoczesnych metod wytwarzania oprogramowania (AI-Pair Programming) udokumentowanych podczas realizacji projektów technologicznych.

---

## 👤 Profil Zawodowy (Podsumowanie do CV)
Inżynier systemów wbudowanych (Embedded Systems Engineer) łączący ponad 10-letnie doświadczenie w architekturze sprzętowej (ARM7, Eclipse) z nowoczesnymi ekosystemami IoT (ESP32, FreeRTOS, PlatformIO). Specjalizuje się w projektowaniu bezpiecznych i optymalnych energetycznie algorytmów sterowania (Hardware-Enforced Safety) oraz efektywnej pracy w paradygmacie AI-Assisted Programming.

---

## 🛠 Filar 1: Architektura Systemów Wbudowanych i Bezpieczeństwo (Embedded Architecture)
*   **Hardware-Enforced Safety:** Świadome projektowanie układów eliminujące błędy software'owe (np. w projekcie `esp32-smart-blinds` konfiguracja przekaźników fizycznie wykluczająca spięcie uzwojeń).
*   **Optymalizacja energii:** Implementacja sekwencera FIFO redukującego prąd szczytowy do ok. 410mA, co umożliwiło zasilanie logiki przez 5V USB.
*   **Modelowanie stanu (Dead Reckoning):** Zmiennoprzecinkowy mechanizm śledzenia pozycji rolet (`millis() / deltaTime`), zapewniający stabilne raportowanie.
*   **Fail-Safe & E-STOP:** Funkcja `emergencyStopAll()` czyszcząca kolejkę zadań i rozłączająca linie zasilające.
*   **Cross-platformowość:** Przeniesienie nawyków zarządzania pamięcią z ARM7 na FreeRTOS/ESP32.

---

## 💻 Filar 2: Inżynieria Oprogramowania i Czysty Kod (Software Engineering & Clean Code)
*   **Refaktoryzacja:** Odseparowanie konfiguracji sprzętowej od logiki sterującej (Designated Initializers).
*   **Izolacja domeny:** Grupowanie urządzeń chroniące pamięć RAM ESP32 przed fragmentacją.
*   **Zarządzanie buildem:** Warunkowe flagi preprocesora dla NVS oraz środowisk testowych (`RUN_TEST_EMERGENCY_STOP`).
*   **Stream Parsers & CLI:** Asynchroniczny `SerialParser` realizujący zasadę *Zero-Blocking Loop Execution*.
*   **Topologie IoT:** Asynchroniczny `WiFiManager` z trybem *Fallback Access Point*.

---

## 👥 Filar 3: Przywództwo Techniczne i Mentoring (Technical Leadership & Mentoring)
*   **Mentoring:** Obniżanie progu wejścia w technologie (Git, algorytmika) i budowanie autonomii zespołu.
*   **Onboarding:** Ścieżki rozwoju oparte na modelowaniu graficznym i Proof of Concept.
*   **Zarządzanie konfiguracją:** Izolowanie stabilnych środowisk (`main`) od gałęzi rozwojowych.

*   ### 🔄 Praktyczne Zastosowanie Metodologii Agile & Scrum (Podejście Hybrydowe)

* **Zwinne Zarządzanie Projektami (Scrum/Agile Framework):** Posiadam teoretyczną i praktyczną wiedzę z zakresu zwinnych metodologii zarządzania, którą z sukcesem implementuję w mikrozespołach inżynierskich. Mapuję książkowe procesy korporacyjne na realne, wysokowydajne działania deweloperskie.
* **Zorientowanie na Sprinty i Przyrost (Sprint-Based Execution):** Pracuję w systemie krótkich, wysoce efektywnych interwałów (Sprintów), nakierowanych na dostarczenie konkretnego, działającego przyrostu oprogramowania (Minimum Viable Product / Increment). Eliminuje to straty czasu na przełączanie kontekstu (Context Switching) i optymalizuje wydajność zespołu.
* **Rola Scrum Mastera i Product Ownera w Praktyce:** W małych zespołach hybrydowych (współpraca człowiek-człowiek-AI) łączę funkcje planowania architektury, definiowania wymagań biznesowych (Product Owner) oraz dbania o płynność i eliminację barier technologicznych (Scrum Master).

---

## 🤖 Filar 4: Nowoczesne Metody Wytwarzania i Automatyzacja (AI-Driven Development & Automation)
*   **AI-Pair Programming:** Delegowanie zadań do modeli AI oraz zarządzanie kontekstem (`AI_context.md`).
*   **Feedback Loops:** Automatyzacja zbierania danych wejściowych i dynamiczne dostosowywanie zachowania systemów.
*   **AI Research:** Walidacja możliwości AI w nietypowych scenariuszach inżynierskich (*Technical Writing & Content Creation*).
