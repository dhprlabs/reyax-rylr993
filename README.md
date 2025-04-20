# LoRa Transceiver Modules



A pair of example projects demonstrating LoRa communication using Reyax RYLR-993 Lite modules:

- **Receiver**: Python script running on a Raspberry Pi 4 (Ubuntu 22.04).
- **Sender**: Arduino/ESP32 sketch.

---

## Table of Contents

- [Overview](#overview)
- [Repository Structure](#repository-structure)
- [Prerequisites](#prerequisites)
- [Hardware Setup](#hardware-setup)
  - [Raspberry Pi 4 Receiver](#raspberry-pi-4-receiver)
  - [ESP32 Sender](#esp32-sender)
- [Software Setup](#software-setup)
- [Testing UART with Minicom](#testing-uart-with-minicom)
- [Running the Receiver](#running-the-receiver)
- [Flashing the ESP32 Sender](#flashing-the-esp32-sender)
- [MQTT Integration (Optional)](#mqtt-integration-optional)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

This repository contains code and wiring details for a LoRa-based transceiver system:

- **Receiver**: Listens for LoRa packets on Raspberry Pi 5 and can forward them to MQTT.
- **Sender**: Periodically transmits messages from an ESP32 board.

---

## Repository Structure

```plain
.
├── assets/
│   ├── rpi4.png                 # rpi4 pinout diagram
│   └── esp32.webp               # esp32 pinout diagram
│
├── receiver/
│   ├── loraReceiver.py          # Python LoRa receiver script
│   ├── requirements.txt         # pip dependencies
│
├── sender/
│   ├── LoRaSender.ino           # ESP32 LoRa sender sketch
│   └── wiring.md                # Detailed ESP32 wiring notes
│
└── README.md                    # This file
```

To include your own photos or diagrams, place them in `images/` and reference them via Markdown:

```markdown
![My Diagram](images/my-diagram.png)
```

---

## Prerequisites

### Hardware

- 2× Reyax RYLR-993 Lite LoRa modules (SX1276)
- Raspberry Pi 4 (Ubuntu 22.04)
- ESP32 development board
- Jumper wires, breadboard, 868 MHz antennas

### Software

- **Ubuntu 22.04 LTS** on Raspberry Pi 4
- **Python 3.11+** and `python3-serial`
- `minicom` (for UART testing)
- **Arduino IDE** (or PlatformIO) with ESP32 support

---

## Hardware Setup

### Raspberry Pi 4 Receiver



| RYLR-993 Pin | Signal      | Pi Header Pin | GPIO Name    |
| ------------ | ----------- | ------------- | ------------ |
| VCC (3.3 V)  | Power input | 1             | —            |
| GND          | Ground      | 6             | —            |
| TXD          | Module TX → | 10            | GPIO15 (RXD) |
| RXD          | Module RX ← | 8             | GPIO14 (TXD) |

See `receiver/wiring.md` for detailed photos and notes.

### ESP32 Sender



| RYLR-993 Pin | Signal      | ESP32 Pin    |
| ------------ | ----------- | ------------ |
| VCC (3.3 V)  | Power input | 3V3          |
| GND          | Ground      | GND          |
| TXD          | Module TX → | GPIO16 (RX2) |
| RXD          | Module RX ← | GPIO17 (TX2) |

See `sender/wiring.md` for detailed photos and notes.

---

## Software Setup

1. **Enable UART** on Ubuntu:

   ```bash
   sudo nano /boot/firmware/config.txt
   # add:
   enable_uart=1
   dtoverlay=disable-bt

   sudo nano /boot/firmware/cmdline.txt
   # remove any console=serial0,115200 entries
   sudo reboot
   ```

2. **Install dependencies**:

   ```bash
   sudo apt update
   sudo apt install python3-serial minicom -y
   pip3 install --user paho-mqtt
   ```

3. **Verify serial port**:

   ```bash
   ls -l /dev/serial0    # should symlink to ttyAMA0
   ```

---

## Testing UART with Minicom

```bash
sudo minicom -b 9600 -D /dev/serial0
```

- Type `AT` + Enter → should return `OK`.
- Exit: `Ctrl+A` then `X`.

---

## Running the Receiver

```bash
cd receiver
python3 loraReceiver.py
```

Watch the console for incoming LoRa packets and optional MQTT publishes.

---

## Flashing the ESP32 Sender

1. Open `sender/LoRaSender.ino` in Arduino IDE or PlatformIO.
2. Select your ESP32 board & port.
3. Upload and open Serial Monitor at 115200 baud.
4. Observe AT commands and `+RCV` responses.

---

## MQTT Integration (Optional)

In `receiver/loraReceiver.py`, configure:

```python
MQTT_BROKER = '127.0.0.1' # your ip address
MQTT_TOPIC  = '/data'
```

Install with `pip3 install --user paho-mqtt` and run.

---

## Contributing

Pull requests and issues are welcome. Please ensure:

- Code is linted and formatted.
- Any added hardware diagrams are placed in `assets/`.
- README is updated with new steps.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

Proficiency Hub ([proficiencyhub95@gmail.com](mailto\:proficiencyhub95@gmail.com))

