Przeanalizujmy ten wzorzec zachowania – jest on niezwykle popularny w sterownikach radiowych i systemach automatyki roletowej:
• Jeśli roleta stoi: Naciśnięcie GÓRA jedzie w górę, naciśnięcie DÓŁ jedzie w dół.
• Jeśli roleta jedzie w GÓRĘ:
	• Ponowne naciśnięcie GÓRA jest ignorowane (roleta kontynuuje ruch).
	• Naciśnięcie przycisku przeciwnego (DÓŁ) powoduje zatrzymanie rolety (STOPPED).
• Jeśli roleta jedzie w DÓŁ:
	• Ponowne naciśnięcie DÓŁ jest ignorowane (roleta kontynuuje ruch).
	• Naciśnięcie przycisku przeciwnego (GÓRA) powoduje zatrzymanie rolety (STOPPED).
Dzięki temu użytkownik może "klikać" ten sam przycisk bez obawy, że przypadkowo zatrzyma roletę, a zmiana kierunku lub zatrzymanie wymaga świadomego kliknięcia drugiego klawisza.

// --- Zmodyfikowane Funkcje Callback dla Przycisków ---

void handleUpButton() {
    BlindsState currentState = blinds.getState();

    if (currentState == STOPPED) {
        // Jeśli roleta stoi, naciśnięcie GÓRA uruchamia ruch w górę
        blinds.moveUp();
    } 
    else if (currentState == MOVING_DOWN) {
        // Jeśli roleta jedzie w DÓŁ, naciśnięcie przeciwnego (GÓRA) zatrzymuje ją
        blinds.stop();
    }
    // Jeśli currentState == MOVING_UP, naciśnięcie tego samego przycisku nic nie robi (ignorujemy)
}

void handleDownButton() {
    BlindsState currentState = blinds.getState();

    if (currentState == STOPPED) {
        // Jeśli roleta stoi, naciśnięcie DÓŁ uruchamia ruch w dół
        blinds.moveDown();
    } 
    else if (currentState == MOVING_UP) {
        // Jeśli roleta jedzie w GÓRĘ, naciśnięcie przeciwnego (DÓŁ) zatrzymuje ją
        blinds.stop();
    }
    // Jeśli currentState == MOVING_DOWN, naciśnięcie tego samego przycisku nic nie robi (ignorujemy)
}
