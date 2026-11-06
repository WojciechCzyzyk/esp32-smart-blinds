#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <optional>
#include <string_view>

enum class BlindState : uint8_t {
    STOPPED,
    MOVING_UP,
    MOVING_DOWN
};

class BlindsManager {
private:
    uint8_t _id;
    BlindState _state{BlindState::STOPPED};
    
    // Serce matematyczne (float dla precyzji delty)
    float _exactPosition{0.0f};
    uint32_t _currentPositionPct{0};
    uint32_t _moveTimeMs;
    uint32_t _lastUpdateTime{0};
    uint32_t _movementStartTime{0}; // Czas wirtualnego startu
    
    // Kalibracja
    bool _isCalibrating{false};
    uint32_t _calibrationStartTime{0};

    // Piny fizyczne (para: Zasilanie + Kierunek)
    uint8_t _pinPower;
    uint8_t _pinDirection;

    // Pamięć NVS
    Preferences _prefs;
    static constexpr std::string_view PREFS_NAMESPACE = "blinds_state";

    void writeHardwarePins(bool powerActive, bool directionUp);
    void saveToNVS();
    void loadFromNVS();

public:
    BlindsManager(uint8_t id, uint8_t pinPower, uint8_t pinDirection, uint32_t defaultMoveTimeMs);

    void begin();
    void moveUp();
    void moveDown();
    void stop();
    void stopPulse(); // Rozłącza przekaźnik po 1s, ale zostawia wirtualny ruch w tle
    void update();    // Aktualizuje maszynę stanów i pozycję %

    void startCalibration(bool up);
    void saveCalibration();

    // Gettery (C++20 std::optional)
    std::optional<uint8_t> getPercentage() const;
    BlindState getState() const { return _state; }
    uint32_t getMoveTime() const { return _moveTimeMs; }
    bool isMoving() const { return _state != BlindState::STOPPED; }
};
