# Wybór Środowiska Mobilnego: Dlaczego Flutter i Dart?

W celu zapewnienia bezprzewodowego sterowania roletami z poziomu smartfona, przeanalizowano dostępne technologie mobilne pod kątem progu wejścia oraz perspektyw rozwoju projektu. 

## Decyzja technologiczna: Flutter (Google)

Do realizacji warstwy mobilnej wybrano framework **Flutter** oraz język **Dart**. Decyzja ta opiera się na następujących kryteriach:

1. **Uniwersalność (Cross-platform):** Jeden kod źródłowy pozwala na wygenerowanie aplikacji na systemy Android oraz iOS, co w przyszłości pozwoli na łatwe rozszerzenie bazy użytkowników systemu Smart Blinds.
2. **Niski próg wejścia dla programistów Embedded:** Język Dart wykazuje silne podobieństwo strukturalne do języków obiektowych takich jak C++ czy Java. Składnia jest intuicyjna dla osoby posiadającej doświadczenie z mikrokontrolerami.
3. **Nowoczesny interfejs użytkownika (UI):** Flutter operuje na architekturze deklaratywnej i gotowych komponentach (Widgets), co pozwala na szybkie i estetyczne zaprojektowanie panelu sterowania roletami bez zaawansowanej wiedzy graficznej.
4. **Wydajność:** Kod jest kompilowany bezpośrednio do natywnego kodu maszynowego procesora w telefonie, co zapewnia płynne działanie animacji i stabilną komunikację sieciową (HTTP/MQTT/WebSocket) z ESP32.
