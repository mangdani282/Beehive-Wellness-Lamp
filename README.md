# Beehive Wellness Lamp

**An Arduino-powered light therapy device that combines wellness technology with nature-inspired design to combat Seasonal Affective Disorder.**

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat-square&logo=Arduino&logoColor=white)
![TinkerCAD](https://img.shields.io/badge/TinkerCAD-1C7ED6?style=flat-square&logo=autodesk&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)
![Status](https://img.shields.io/badge/status-prototype-orange?style=flat-square)

## Overview

The Beehive Wellness Lamp delivers targeted light therapy through an Arduino-controlled system housed in a bee-themed enclosure. This project demonstrates embedded systems programming, circuit design, and therapeutic device development using accessible maker tools.

The lamp addresses Seasonal Affective Disorder by providing controlled light exposure with customizable intensity and timing patterns. The electronics and control logic showcase practical Arduino applications in health technology.

## Features

- **Arduino-based light control** - Programmable brightness and timing sequences
- **Therapeutic light spectrum** - Optimized wavelength output for SAD treatment
- **Custom circuit design** - Purpose-built electronics layout in TinkerCAD
- **Modular architecture** - Separate power, control, and lighting circuits
- **User-friendly interface** - Simple controls for therapy session management
- **Bee-inspired aesthetics** - Nature-themed design promotes wellness connection

## Electronics Design

The system uses Arduino microcontroller architecture with the following components:

**Core Electronics:**
- Arduino microcontroller for light pattern control
- LED array with appropriate current limiting resistors
- Power regulation circuit for stable operation
- User input interface (buttons/potentiometer)
- Timer-based therapy session control

**Circuit Features:**
- PWM brightness control for smooth light transitions
- Multiple lighting modes (constant, pulsing, sunrise simulation)
- Safety features including thermal protection
- Low-power sleep mode between sessions

## Technical Implementation

### Hardware Setup
```
Arduino Uno/Nano
├── Digital Pins 3,5,6,9,10,11 → PWM LED Control
├── Digital Pins 2,4,7 → User Input Buttons
├── Analog Pin A0 → Brightness Potentiometer
└── Digital Pin 12 → Status Indicator LED
```

### Key Code Functions
- **Light Therapy Modes**: Multiple preset patterns for different treatment needs
- **Session Timer**: Automatic therapy duration control with gradual fade-in/fade-out
- **User Interface**: Button-based mode selection and manual brightness adjustment
- **Power Management**: Sleep modes and efficient LED driving for battery operation

## Getting Started

### Prerequisites
- Arduino IDE (version 1.8.0 or higher)
- Arduino Uno or compatible microcontroller
- Basic electronic components (LEDs, resistors, buttons)
- Breadboard or PCB for circuit assembly

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/mangdani282/Beehive-Wellness-Lamp.git
   cd Beehive-Wellness-Lamp
   ```

2. **Open in Arduino IDE**
   ```bash
   # Open the main .ino file in Arduino IDE
   # Select your Arduino board and COM port
   ```

3. **Upload to Arduino**
   - Connect your Arduino via USB
   - Verify the code compiles without errors
   - Upload to your microcontroller

### Circuit Assembly

1. **View the TinkerCAD design**: [Circuit Layout](https://www.tinkercad.com/things/lhDUy1F77gH-team-1-wellness-lamp)
2. **Assemble components** according to the circuit diagram
3. **Test basic LED functionality** before final assembly
4. **Integrate with diffuser housing** (3D printed separately)

## Usage

### Basic Operation
```cpp
// Power on the device
// Press Mode button to cycle through therapy settings:
// - Morning Boost (15 min, high intensity)
// - Evening Wind-down (30 min, warm spectrum)
// - Custom Session (user-defined duration/brightness)
```

### Therapy Sessions
- **Morning routine**: 10,000 lux equivalent for 20-30 minutes
- **Maintenance**: Lower intensity sessions throughout the day  
- **Evening use**: Warm light transition to support sleep cycle

## Tech Stack

**Hardware:**
- Arduino Uno/Nano microcontroller
- High-efficiency LED arrays
- Analog input sensors
- Digital control interfaces

**Software:**
- Arduino C++ programming
- PWM control algorithms
- Timer-based state machines
- Power optimization routines

**Design Tools:**
- TinkerCAD for circuit simulation and layout
- Arduino IDE for firmware development
- Onshape for mechanical design (diffuser housing)

## Project Resources

- **[Live Demo & Presentation](https://www.canva.com/design/DAGzQxjnuIQ/bCZ2edxNf6E_iI4nAj082w/edit?utm_content=DAGzQxjnuIQ&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton)**: Complete project overview with working demonstration
- **[TinkerCAD Circuit](https://www.tinkercad.com/things/lhDUy1F77gH-team-1-wellness-lamp?sharecode=S_ln49739dWDfW8EBe-C1xNB4H2MaLQpl5_Rckbl5nw)**: Interactive electronics design and simulation

## Contributing

Contributions welcome! This project demonstrates embedded systems fundamentals with practical health applications.

**Development areas:**
- Additional therapy light patterns
- Mobile app integration via Bluetooth
- Advanced timer and scheduling features
- Power consumption optimizations
- PCB design for production-ready assembly

**To contribute:**
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Test your changes thoroughly with hardware
4. Submit a pull request with clear documentation

## License

This project is open source and available under the [MIT License](LICENSE).

## Acknowledgments

Created as part of a wellness technology project focusing on accessible SAD treatment solutions through maker electronics and embedded programming.

---

*This project showcases practical Arduino development, circuit design, and therapeutic device programming using industry-standard maker tools and development practices.*
