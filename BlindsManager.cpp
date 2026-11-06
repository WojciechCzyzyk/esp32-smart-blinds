#include "BlindsManager.h"

BlindsManager::BlindsManager(uint8_t id, uint8_t pinPower, uint8_t pinDirection, uint32_t defaultMoveTimeMs)
    : _id(id), _moveTimeMs(defaultMoveTimeMs), _pinPower(pinPower), _pinDirection(pinDirection) {}

void BlindsManager::begin() {
    pinMode(_pinPower, OUTPUT);
    pinMode(_pinDirection, OUTPUT);
    writeHardwarePins(false, false); // Stan bezpieczny spoczynkowy
    loadFromNVS();
}

void BlindsManager::writeHardwarePins(bool powerActive, bool directionUp) {
    // Power: LOW = przekaźnik załączony, HIGH = rozłączony (bezpieczny)
    // Direction: LOW = GÓRA, HIGH = DÓŁ
    digitalWrite(_pinDirection, directionUp ? LOW : HIGH);
    delay(50); // Sprzętowy interlock czasowy
    digitalWrite(_pinPower, powerActive ? LOW : HIGH);
}

void BlindsManager::moveUp() {
    if (isMoving()) stop();

    _state = BlindState::MOVING_UP;
    _lastUpdateTime = millis();
    _movementStartTime = _lastUpdateTime;
    writeHardwarePins(true, true);
    Serial.printf("[Roleta %d] Start w GÓRĘ od pozycji: %d%%\n", _id, _currentPositionPct);
}

void BlindsManager::moveDown() {
    if (isMoving()) stop();

    _state = BlindState::MOVING_DOWN;
    _lastUpdateTime = millis();
    _movementStartTime = _lastUpdateTime;
    writeHardwarePins(true, false);
    Serial.printf("[Roleta %d] Start w DÓŁ od pozycji: %d%%\n", _id, _currentPositionPct);
}

void BlindsManager::stopPulse() {
    // Odcinamy zasilanie cewki przekaźnika (0 prądu z USB)
    digitalWrite(_pinPower, HIGH);
    Serial.printf("[Roleta %d] Impuls 1s zakończony. Przekaźnik rozłączony. Ruch w tle.\n", _id);
}

void BlindsManager::stop() {
    if (_state == BlindState::STOPPED) return;

    writeHardwarePins(false, false); // Fizyczny reset wyjść
    update(); // Ostatnie doliczenie pozycji przed zatrzymaniem
    _state = BlindState::STOPPED;

    Serial.printf("[Roleta %d] Zatrzymano ostatecznie na pozycji: %d%%\n", _id, _currentPositionPct);
    saveToNVS();
}

void BlindsManager::update() {
    if (_state == BlindState::STOPPED) return;

    uint32_t now = millis();
    uint32_t deltaTime = now - _lastUpdateTime;
    _lastUpdateTime = now;

    // 1. Zabezpieczenie przed przekroczeniem maksymalnego wirtualnego czasu przejazdu
    if (!_isCalibrating && (now - _movementStartTime >= _moveTimeMs)) {
        if (_state == BlindState::MOVING_UP) _exactPosition = 100.0f;
        if (_state == BlindState::MOVING_DOWN) _exactPosition = 0.0f;
        
        _state = BlindState::STOPPED;
        writeHardwarePins(false, false);
        _currentPositionPct = static_cast<uint32_t>(_exactPosition);
        Serial.printf("[Roleta %d] Osiągnięto wirtualną krańcówkę czasową. Stop.\n", _id);
        saveToNVS();
        return;
    }

    // 2. Matematyka wyliczania pozycji delty (Twoje precyzyjne serce na float)
    float deltaPercent = (static_cast<float>(deltaTime) / static_cast<float>(_moveTimeMs)) * 100.0f;

    if (_state == BlindState::MOVING_UP) {
        _exactPosition += deltaPercent;
        if (_exactPosition > 100.0f) _exactPosition = 100.0f;
    } else if (_state == BlindState::MOVING_DOWN) {
        _exactPosition -= deltaPercent;
        if (_exactPosition < 0.0f) _exactPosition = 0.0f;
    }

    _currentPositionPct = static_cast<uint32_t>(_exactPosition + 0.5f);
}

void BlindsManager::startCalibration(bool up) {
    Serial.printf("[Roleta %d] Rozpoczęcie kalibracji stoperem.\n", _id);
    _isCalibrating = true;
    _calibrationStartTime = millis();
    if (up) moveUp(); else moveDown();
}

void BlindsManager::saveCalibration() {
    if (!_isCalibrating || !isMoving()) return;

    uint32_t duration = millis() - _calibrationStartTime;
    _isCalibrating = false;
    stop();

    _moveTimeMs = duration;
    Serial.printf("[Roleta %d] Kalibracja udana. Nowy czas: %lu ms\n", _id, _moveTimeMs);
    saveToNVS();
}

std::optional<uint8_t> BlindsManager::getPercentage() const {
    if (_moveTimeMs == 0) return std::nullopt;
    return static_cast<uint8_t>(_currentPositionPct);
}

void BlindsManager::saveToNVS() {
    _prefs.begin(PREFS_NAMESPACE.data(), false);
    char keyPos[16];
    char keyTime[16];
    snprintf(keyPos, sizeof(keyPos), "pos_%d", _id);
    snprintf(keyTime, sizeof(keyTime), "time_%d", _id);

    _prefs.putInt(keyPos, _currentPositionPct);
    _prefs.putULong(keyTime, _moveTimeMs);
    _prefs.end();
}

void BlindsManager::loadFromNVS() {
    _prefs.begin(PREFS_NAMESPACE.data(), true);
    char keyPos[16];
    char keyTime[16];
    snprintf(keyPos, sizeof(keyPos), "pos_%d", _id);
    snprintf(keyTime, sizeof(keyTime), "time_%d", _id);

    _currentPositionPct = _prefs.getInt(keyPos, 0);
    _exactPosition = static_cast<float>(_currentPositionPct);
    _moveTimeMs = _prefs.getULong(keyTime, _moveTimeMs);
    _prefs.end();
}
