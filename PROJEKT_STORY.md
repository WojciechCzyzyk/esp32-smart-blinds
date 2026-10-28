# Droga Architekta: Reaktywacja, Mentoring i Praca w Zespole Hybrydowym (Wojciech Czyżyk)

*„Ten dokument to nie jest zwykłe portfolio. To żywy log z transformacji inżyniera z 10-letnim doświadczeniem w ARM, który po resetującym urlopie i ciężkiej operacji kręgosłupa wrócił do gry, by stworzyć zespół hybrydowy (Człowiek-Człowiek-AI) i wciągnąć swojego 11-letniego syna do świata nowoczesnego IT.”*

---

## 🪵 Rozdział 1: Samotny Wilk z czasów ARM vs. Praca w Zespole (Stan Psychologiczny: Przełom)
Przez ponad dekadę projektowałem systemy wbudowane w pojedynkę. Moim światem były mikrokontrolery ARM7, środowisko Eclipse i bezpośrednia walka z rejestrami. Czułem się w tym pewnie, ale rynek poszedł do przodu. 
*   **Emocje:** Obawa przed wejściem w nowe ekosystemy IoT vs. potężny głód inżynierski po długim restarcie.
*   **Punkt Zwrotny:** Odkrycie potęgi Sztucznej Inteligencji jako pełnoprawnego partnera w zespole deweloperskim (AI-Pair Programming). Zrozumiałem, że nie muszę już pracować sam.

## 👥 Rozdział 2: Narodziny Zespołu i Pierwszy Sprint (Rola: Technical Leader & Scrum Master)
Zamiast rzucić mojego 11-letniego syna Bartka na głęboką wodę i ryzykować jego frustrację, zastosowałem rasową metodologię Scrum:
1.  **Proof of Concept (PoC):** Najpierw sam, we współpracy z AI, przetestowałem wdrożenie strony na GitHub Pages i integrację ze Scratchem.
2.  **Wdrożenie (Onboarding):** Usiadłem z Bartkiem jako Mentor. Wyeliminowałem barierę konsoli (użyliśmy wyłącznie GitHub Desktop). 
*   **Sukces Emocjonalny (200% normy):** Bartek odpalił swoją stronę, wrzucił tam swoje gry i filmiki. Gdy miał problem ze sprawdzianem z dzielenia z resztą, napisaliśmy w Scratchu zabawny kalkulator rozbijający algorytm na kroki. Chłopak zyskał potężną pewność siebie przed rówieśnikami i wymagającą nauczycielką. Zobaczył, że bariery są po to, by je mądrze obchodzić.

## 🔬 Rozdział 3: Twarde Przypadki Inżynierskie w "Smart Blinds" (Stan Techniczny: Głęboka Architektura)
Tutaj opisujemy nasze największe walki z projektem rolet, które będziemy na bieżąco uzupełniać o czysty kod:
*   **Case 1: Hardware-Enforced Safety.** Jak konfiguracja podwójnych przekaźników fizycznie zabezpieczyła silnik przed jednoczesnym podaniem napięcia na oba uzwojenia (eliminacja błędów software'owych).
*   **Case 2: Optymalizacja Prądu Szczytowego.** Wdrożenie sekwencera FIFO, który ściął pik prądowy do 410mA, ratując logikę ESP32 przed resetami.
