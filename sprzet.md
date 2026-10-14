# Projekt: Sterownik Rolet ESP32 (8 rolet / 16 przekaźników)

## Wybrany Sprzęt:
* **Mikrokontroler:** ESP32 ESP-WROOM-32D (WiFi/Bluetooth, logika 3.3V)
* **Akcesoria:** Adapter Shield ze złączami śrubowymi ARK, płytka stykowa 400 pól, przewody M-M 20cm
* **Wykonawstwo:** 2x Moduł przekaźnika 8 kanałów z optoizolacją 5V 10A L/H (wyzwalanie ustawione na HIGH)
* **Sterowanie rolet:** Niskonapięciowe sygnałowe (~32V DC, magistrala Nice TTBus)

## Założenia projektowe i zasilanie:
* Zasilanie całości z jednej ładowarki telefonicznej USB (5V / min 2.4A).
* Sterowanie przekaźnikami w kodzie sekwencyjnie (maksymalnie 2 przekaźniki naraz), aby ograniczyć pobór prądu w szczycie do ok. 410mA.


## Mapa pinów GPIO (ESP32 ESP-WROOM-32D)

### Moduł Przekaźników 1 (Rolety 1 - 4)
* **Roleta 1:** GPIO 13 (Zasilanie ON/OFF) | GPIO 12 (Kierunek GÓRA/DÓŁ)
* **Roleta 2:** GPIO 14 (Zasilanie ON/OFF) | GPIO 27 (Kierunek GÓRA/DÓŁ)
* **Roleta 3:** GPIO 26 (Zasilanie ON/OFF) | GPIO 25 (Kierunek GÓRA/DÓŁ)
* **Roleta 4:** GPIO 33 (Zasilanie ON/OFF) | GPIO 32 (Kierunek GÓRA/DÓŁ)

### Moduł Przekaźników 2 (Rolety 5 - 8)
* **Roleta 5:** GPIO 15 (Zasilanie ON/OFF) | GPIO 2 (Kierunek GÓRA/DÓŁ)
* **Roleta 6:** GPIO 4 (Zasilanie ON/OFF)  | GPIO 16 (Kierunek GÓRA/DÓŁ)
* **Roleta 7:** GPIO 17 (Zasilanie ON/OFF) | GPIO 5 (Kierunek GÓRA/DÓŁ)
* **Roleta 8:** GPIO 18 (Zasilanie ON/OFF) | GPIO 19 (Kierunek GÓRA/DÓŁ)

### Piny zarezerwowane / wolne:
* **GPIO 1 & GPIO 3:** Debugowanie (TX0/RX0 przez USB)
* **GPIO 34, 35, 36, 39:** Wolne piny (tylko wejściowe)

