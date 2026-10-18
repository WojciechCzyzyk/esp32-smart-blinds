# Portfolio Kompetencji i Współpracy z AI (AI-Assisted Engineering)

Ten plik stanowi rejestr zaawansowanych kompetencji inżynierskich, architektonicznych oraz nowoczesnych metod wytwarzania oprogramowania (AI-Pair Programming) udokumentowanych podczas realizacji projektu `esp32-smart-blinds`.

## 1. Nowoczesne metody wytwarzania oprogramowania (AI / Prompt Engineering)

* **Kompetencja:** Efektywne współdzielenie pracy z Agentami AI (AI-Driven Development / AI-Pair Programming).
* **Dowód w projekcie:** Umiejętność precyzyjnego delegowania zadań implementacyjnych do AI na podstawie dostarczonej dokumentacji i koncepcji architektonicznych. Zdolność do jasnego formułowania wymagań funkcjonalnych, weryfikacji kodu dostarczonego przez model oraz szybkiej adaptacji wzorców projektowych do specyfiki mikrokontrolerów ESP32.

## 2. Architektura Systemów Wbudowanych i Bezpieczeństwo (Embedded Architecture)

* **Kompetencja:** Projektowanie systemów wbudowanych zorientowanych na bezpieczeństwo sprzętowe (Hardware-Enforced Safety).
* **Dowód w projekcie:** Świadome zaprojektowanie unikalnego układu dwóch przekaźników (Zasilanie + Kierunek) per sekcja. Rozwiązanie to fizycznie wyklucza ryzyko jednoczesnego podania napięcia na uzwojenia przeciwbieżne silnika rolety, eliminując błędy ludzkie na poziomie software'owym.
* **Doświadczenie cross-platformowe:** Skuteczne przeniesienie rygorystycznych nawyków niskopoziomowego zarządzania pamięcią i rejestrami (z platform ARM7/Eclipse) na nowoczesne systemy operacyjne czasu rzeczywistego (FreeRTOS/ESP32) przy zachowaniu optymalizacji kodu.

## 3. Optymalizacja i Zarządzanie Zasobami (Resource Management)

* **Kompetencja:** Projektowanie algorytmów deterministycznych pod restrykcyjne ograniczenia budżetu energetycznego.
* **Dowód w projekcie:** Koncepcja sekwencera zadań (kolejka FIFO) pozwalająca na redukcję prądu szczytowego całego systemu do poziomu pojedynczego silnika (ok. 410mA). Pozwoliło to na rezygnację z drogich transformatorów na rzecz lekkiego zasilania logiki przez 5V USB.

## 4. Czysty Kod i Skalowalność (Clean Code & Maintainability)

* **Kompetencja:** Refaktoryzacja kodu w celu uzyskania wysokiej modularności (Data-Driven Design).
* **Dowód w projekcie:** Inicjatywa pełnego odseparowania warstwy konfiguracji sprzętowej od logiki klas sterujących. Zastosowanie struktur konfiguracyjnych i jawnej indeksacji tablic (Designated Initializers) w celu eliminacji tzw. "magicznych liczb", co pozwala na bezbłędne rekonfigurowanie pinów i czasów pracy urządzeń w jednym miejscu pliku `main.cpp`.

## 5. Projektowanie Interfejsów i Logiki Strefowej (Domain-Driven State Isolation)

* **Kompetencja:** Projektowanie logicznych stref sterowania (Zone Control) w architekturze rozproszonej bez powielania zasobów sprzętowych.
* **Dowód w projekcie:** Implementacja algorytmu grupowania urządzeń wykonawczych (funkcje `closeAll`, `controlZoneSalonKuchnia`). Zamiast tworzenia kosztownych i skomplikowanych obiektów "grup", sterowanie oparto na dynamicznym kolejkowaniu ID istniejących instancji fizycznych. Chroni to ograniczoną pamięć RAM mikrokontrolera ESP32 przed fragmentacją i wyciekami pamięci, zachowując jednocześnie pełną elastyczność konfiguracji przestrzeni mieszkalnej dla użytkownika końcowego.


## 6. Zaawansowane Zarządzanie Buildem i Kompilacja Warunkowa (Advanced Build Configuration)

* **Kompetencja:** Implementacja mechanizmów kompilacji warunkowej (Conditional Compilation) w systemach pozbawionych pełnego sprzężenia zwrotnego (Sensorless Systems).
* **Dowód w projekcie:** Zaprojektowanie i wdrożenie opcjonalnego podsystemu zapisu stanu urządzeń w pamięci nieulotnej NVS (`<Preferences.h>`) sterowanego poziomem flag preprocesora (`-D USE_NVS_PREFERENCES`) z poziomu konfiguracji PlatformIO. Świadome zidentyfikowanie ryzyka desynchronizacji stanów logicznych z fizycznymi (w przypadku manualnej zmiany pozycji rolet przełącznikami ściennymi) i dostarczenie architektury umożliwiającej bezinwazyjne włączanie/wyłączanie tej funkcji bez modyfikacji kodu źródłowego.

## 7. Matematyczne Modelowanie Stanu Urządzeń Wykonawczych (Dead Reckoning Positioning)

* **Kompetencja:** Implementacja algorytmów estymacji pozycji bezczujnikowej (Dead Reckoning) przy użyciu struktur czasu rzeczywistego.
* **Dowód w projekcie:** Zaprojektowanie zmiennoprzecinkowego mechanizmu śledzenia pozycji rolet w skali 0–100% na podstawie różnicowych pomiarów czasu (`millis() / deltaTime`). Zastosowanie odseparowanej akumulacji zmiennej `exactPosition` typu `float` wyeliminowało błędy zaokrągleń stałoprzecinkowych przy wysokich częstotliwościach taktowania pętli głównej, zapewniając stabilne i powtarzalne raportowanie stanu rolet do systemów nadrzędnych.

## 8. Dojrzałość Inżynierska i Adaptacja Nowych Technologii (Seniority & Tech Adaptability)

* **Kompetencja:** Wykorzystanie głębokiej, fundamentalnej wiedzy niskopoziomowej do błyskawicznej implementacji nowoczesnych frameworków i narzędzi deweloperskich.
* **Profil inżynierski:** Zdolność do natychmiastowego mapowania nowoczesnych warstw abstrakcji (takich jak biblioteki ESP32, PlatformIO, systemy baz klucz-wartość NVS) na fundamentalne zasoby sprzętowe (mikrokontrolery, pamięci Flash/EEPROM, rejestry). 
* **Wartość dla projektów:** Doświadczenie w manualnym tworzeniu rozwiązań systemowych w środowiskach takich jak Eclipse/ARM7 pozwala na głębokie zrozumienie "dlaczego" i "jak" działają współczesne mechanizmy wysokiego poziomu. Przekłada się to na projektowanie kodu, który jest nie tylko szybki w implementacji, ale przede wszystkim bezpieczny, przewidywalny sprzętowo i zoptymalizowany pod kątem zużycia zasobów (Resource-Aware Development). Posiada silną wewnętrzną motywację do ciągłego rozwoju i analitycznego rozwiązywania złożonych problemów technicznych.

## 9. Projektowanie Systemów Krytycznych i Obsługa Stanów Alarmowych (Fail-Safe & E-STOP Design)

* **Kompetencja:** Implementacja wielopoziomowych mechanizmów bezpieczeństwa (Fail-Safe) i natychmiastowego zatrzymania procesów fizycznych (Emergency Stop).
* **Dowód w projekcie:** Zaprojektowanie i wdrożenie funkcji `emergencyStopAll()`. Algorytm realizuje jednoczesne czyszczenie pamięci kolejki zadań FIFO (poprzez optymalną kosztowo zamianę struktur `std::swap`) oraz wymusza bezwarunkowe, prewencyjne rozłączenie wszystkich linii zasilających przekaźników wykonawczych. Zapewnia to natychmiastowe sprowadzenie systemu sterowania budynkiem do stanu bezpiecznego (zero poboru prądu przez silniki) w przypadku wykrycia zagrożenia.

## 10. Architektura Autokalibracji i Zarządzanie Profilami Sprzętowymi (Dynamic Hardware Profiling)

* **Kompetencja:** Projektowanie runtime'owych systemów autokalibracji (Self-Calibration) z dynamicznym utrwalaniem stanów (Dynamic Profile Persistence).
* **Dowód w projekcie:** Implementacja podsystemu pomiaru i kalibracji parametrów czasowych silników wykonawczych. Algorytm umożliwia precyzyjne wyznaczenie rzeczywistego czasu pracy rolet (`moveTime`) bezpośrednio na obiekcie, eliminując konieczność sztywnego kodowania wartości w oprogramowaniu układowym (Firmware). Przechowywanie profili w przestrzeni NVS (`putULong`) zapewnia separację konfiguracji fabrycznej od nastaw serwisowych użytkownika, podnosząc uniwersalność wdrożeniową systemu.

## 11. Izolacja Środowisk Testowych na Poziomie Systemu Buildów (Modular Test-Driven Build Profiles)

* **Kompetencja:** Wdrażanie zaawansowanych profili i rusztowań testowych (Test Scaffolding) zintegrowanych z systemem zarządzania projektem (PlatformIO CLI).
* **Dowód w projekcie:** Wprowadzenie niezależnych flag preprocesora (`RUN_TEST_EMERGENCY_STOP`, `RUN_TEST_CALIBRATION`) do konfiguracji środowiskowej. Pozwoliło to na hermetyzację eksperymentalnych scenariuszy wykonawczych w pliku `main.cpp`. Architektura ta umożliwia deweloperowi błyskawiczne włączanie/wyłączanie testów regresji sprzętowej z poziomu pliku `platformio.ini` bez ingerencji w strukturę kodu produkcyjnego, eliminując ryzyko przypadkowego pozostawienia kodu debugującego w wersji finalnej.

## 12. Projektowanie Interfejsów Tekstowych i Parserów Strumieniowych (Stream Parsers & CLI Design)

* **Kompetencja:** Implementacja lekkich, nieblokujących parserów strumieni danych (Stream Parsing) dla interfejsów diagnostyczno-kontrolnych (Command Line Interface).
* **Dowód w projekcie:** Zaprojektowanie i wdrożenie asynchronicznego parsera komend szeregowych `SerialParser`. Zastosowanie buforowania znakowego sterowanego przerwaniami strumienia UART pozwala na bezbłędne dekodowanie instrukcji sterujących (`STOP_ALL`, `SALON_UP`) w czasie rzeczywistym. Architektura ta nie zakłóca pracy pętli głównej mikrokontrolera (Zero-Blocking Loop Execution) oraz izoluje warstwę parsowania od fizycznej logiki wykonawczej sekwencera.

## 13. Projektowanie Architektury Sieciowej IoT i Trybów Awaryjnych (Fail-Safe Network Topologies)

* **Kompetencja:** Implementacja hybrydowych stosów sieciowych Wi-Fi (Dual AP/STA Mode) o wysokiej odporności na błędy infrastruktury zewnętrznej.
* **Dowód w projekcie:** Zaprojektowanie i wdrożenie modułu `WiFiManager` zarządzającego połączeniem bezprzewodowym w sposób asynchroniczny. Zastosowanie deterministycznego mechanizmu kontroli czasu oczekiwania (Connection Timeout) wyeliminowało ryzyko całkowitego zablokowania pętli głównej mikrokontrolera w przypadku awarii domowego routera. Wprowadzenie automatycznego trybu przełączania w stan punktu dostępowego (Fallback Access Point) zapewnia ciągłą dostępność interfejsu konfiguracyjnego i serwisowego urządzenia w każdych warunkach radiowych.


Profil Zawodowy do CV (Wersja Robocza)
Inżynier systemów wbudowanych (Embedded Systems Engineer) łączący rzetelną, fundamentalną wiedzę z zakresu architektury sprzętowej (ARM7, systemy dedykowane) z nowoczesnymi narzędziami szybkiego prototypowania i wdrażania systemów (ESP32, FreeRTOS, PlatformIO). Specjalizuje się w projektowaniu bezpiecznych elektrycznie i optymalnych energetycznie algorytmów sterowania (Hardware-Enforced Safety). Wykazuje wyjątkową zdolność adaptacji nowych technologii oraz efektywnej pracy w paradygmacie AI-Assisted Programming, przekładając koncepcje biznesowe i architektoniczne na niezawodny kod produkcyjny.