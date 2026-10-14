# Smart Blinds Controller (ESP32 & Nice TTBus)

An open-source smart home automation project dedicated to managing 8 roller blinds independently (16 control channels).

## Core Project Features:
- **Microcontroller:** ESP32 (ESP-WROOM-32D) serving as the central processing unit.
- **Electrical Safety:** Hardware-level interlocking relays (split into Power and Direction components) to physically prevent simultaneous Up/Down voltage overheads.
- **Integration:** Directly interfacing with the low-voltage (~32V DC) signal lines of the proprietary Nice TTBus system.
- **Power Optimization:** Sequential task queuing algorithm implemented in code to limit peak power consumption to ~410mA, allowing the entire setup to run off a single 5V USB charger.
- **Mobile Application:** Built with Flutter (Dart) as a cross-platform control panel for Android and iOS devices.

## Project Milestones:
- [x] Electrical installation analysis & hardware component selection (10A L/H Optoisolated Relays).
- [x] Development of a safe GPIO pin map for the ESP32.
- [x] Development environment setup (VS Code + PlatformIO + Flutter) and communication verification.
- [ ] Implementation of the sequencer logic and task queuing.
- [ ] On-site hardware testing and deployment.
- [ ] Mobile application design and implementation in Flutter.


