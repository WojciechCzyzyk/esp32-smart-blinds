# Projekt: Sterownik Rolet ESP32 (8 rolet / 16 przekaźników)

## Wybrany Sprzęt:
* **Mikrokontroler:** ESP32 ESP-WROOM-32D (WiFi/Bluetooth, logika 3.3V)
* **Akcesoria:** Adapter Shield ze złączami śrubowymi ARK, płytka stykowa 400 pól, przewody M-M 20cm
* **Wykonawstwo:** 2x Moduł przekaźnika 8 kanałów z optoizolacją 5V 10A L/H (wyzwalanie ustawione na HIGH)
* **Sterowanie rolet:** Niskonapięciowe sygnałowe (~32V DC, magistrala Nice TTBus)

## Założenia projektowe i zasilanie:
* Zasilanie całości z jednej ładowarki telefonicznej USB (5V / min 2.4A).
* Sterowanie przekaźnikami w kodzie sekwencyjnie (maksymalnie 2 przekaźniki naraz), aby ograniczyć pobór prądu w szczycie do ok. 410mA.
