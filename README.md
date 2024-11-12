# Chip-8 Emulator

This is a **Chip-8 emulator** written in C, designed to emulate the 8-bit Chip-8 virtual machine, a popular system used for early video games. It provides a simulation of the Chip-8 architecture and includes the ability to load and execute Chip-8 programs (ROM files). The emulator includes support for graphics rendering, input handling, and sound output.

## Key Features

- **Multithreaded Audio**: The emulator uses a dedicated audio thread to handle the beep sound. This ensures that audio playback doesn't block the main thread responsible for rendering and handling input events.
- **Beep Sound**: When the sound timer reaches zero, a beep is omitted.
- **SDL2 for Graphics**: The emulator uses SDL2 for rendering the 64x32 pixel display and handles user input through the keyboard.


## Table of Contents

- [Introduction](#introduction)
- [Chip-8 Architecture](#chip-8-architecture)
- [Features](#features)
- [Getting Started](#getting-started)
- [Building the Emulator](#building-the-emulator)
- [Running a Chip-8 ROM](#running-a-chip-8-rom)
- [Control Keys](#control-keys)
- [Limitations](#limitations)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The Chip-8 emulator mimics the operation of the Chip-8 virtual machine, which was originally developed by Joseph Weisbecker in the 1970s. The system was popularized in the 1980s by being used on various hobbyist platforms, notably the COSMAC VIP and Telmac 1800. Chip-8 was a simple yet powerful system for creating early video games, and this emulator aims to preserve that legacy by enabling users to run classic Chip-8 games and programs.

## Chip-8 Architecture

Chip-8 is an interpreted programming language designed for an 8-bit microprocessor. The Chip-8 architecture consists of several key components:

- **Memory**: The system has 4096 bytes of memory, where the first 512 bytes are typically used for system-related functions, and the remaining 3584 bytes are available for programs.
  
- **Registers**: There are 16 8-bit general-purpose registers (V0 to VF). The VF register is often used as a flag for certain operations (e.g., carry flag for arithmetic operations).
  
- **Program Counter (PC)**: The program counter is a 16-bit register that holds the address of the next instruction to be executed. It starts at `0x200` and increases as instructions are fetched.
  
- **Stack**: The system has a 16-level deep stack, used for storing the return addresses during subroutine calls.
  
- **Display**: The Chip-8 display is a 64x32 monochrome grid of pixels, where each pixel can be either on or off (white or black).
  
- **Timers**: Two 8-bit timers (delay and sound timers) are used to manage time-based events. When the timers reach zero, the program can handle these events (e.g., making a sound).
  
- **Input**: The Chip-8 system uses a hexadecimal keypad consisting of 16 keys (0–F), mapped to a 4x4 grid.

## Features

- **Emulation of the Chip-8 Architecture**: The emulator faithfully emulates the core features of the Chip-8 system, including memory, registers, stack, and timers.
  
- **Graphics Rendering**: It uses a simple framebuffer (64x32 grid) to render the output of the Chip-8 program to the screen.
  
- **Input Handling**: The emulator supports the standard Chip-8 input system (16-key hexadecimal keypad) and maps it to corresponding keys on the host system.
  
- **Sound**: The emulator can play sounds using the system's sound timer.
  
- **Instruction Set Support**: All standard Chip-8 instructions are supported, including arithmetic, graphics, input/output, and control flow instructions.
  
- **ROM Support**: It allows users to load and run Chip-8 programs (ROM files) directly in the emulator.
  
- **Optimized for Performance**: The emulator is written to be efficient and works in real-time, enabling smooth execution of Chip-8 programs.
  
- **Customizable**: The code structure is designed to be easily extended with new features and optimizations.

## Getting Started

To use the Chip-8 emulator, you will need to clone this repository and build the project on your local machine.

### Prerequisites

- A C compiler (e.g., GCC or Clang)
- SDL2 library for graphics and input handling (installable via package manager)

On Ubuntu, you can install the dependencies with the following command:

```bash
sudo apt-get install libsdl2-dev
```

## Building the Emulator

1. Clone the repository:

```bash
git clone https://github.com/sudoXpg/chip8emu
cd chip8-emulator
```

2. Compile the source code:

```bash
make
```

3. This will produce an executable binary named `chip8`.

## Running a Chip-8 ROM

Once the emulator is built, you can run a Chip-8 program by passing the ROM file as an argument:

```bash
./chip8 path/to/your/rom.ch8
```

Make sure your ROM file is a valid Chip-8 program.

## Control Keys

The emulator uses a 4x4 grid of keys to represent the Chip-8 keypad, which is mapped as follows:

| Chip-8 Key | Keyboard Key |
|-------------|--------------|
| 1           | 1            |
| 2           | 2            |
| 3           | 3            |
| 4           | 4            |
| 5           | Q            |
| 6           | W            |
| 7           | E            |
| 8           | R            |
| 9           | A            |
| A           | S            |
| B           | D            |
| C           | F            |
| D           | Z            |
| E           | X            |
| F           | C            |

You can interact with the emulator by pressing these keys.


## Tested Using CHIP-8 Test Suite

This CHIP-8 emulator has been tested using the **[CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file#ibm-logo)**. The test suite contains a variety of programs that ensure the accuracy and functionality of the emulator, including:

- **Display Tests**: Verifying proper screen rendering.
- **Instruction Set Tests**: Checking if all CHIP-8 instructions are handled correctly.
- **Sound Tests**: Ensuring the emulator handles audio and timing as expected.
  
The emulator passed a number of these tests successfully, which validates the correctness of its implementation of the CHIP-8 architecture.


## Resources and Help

This CHIP-8 emulator project is inspired by and built upon various resources available online. The following links were particularly helpful in understanding the CHIP-8 architecture and emulator development:

- **[CHIP-8 Architecture](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1)**
  
- **[How to Write a CHIP-8 Emulator](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)**


## Contributing

Contributions are welcome! If you'd like to improve or extend the emulator, feel free to fork the repository and submit pull requests. Please ensure that your contributions follow the existing code style and include adequate comments and documentation.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

Feel free to adjust the content based on your actual implementation details and specific features. This template should give your project a professional appearance and clearly explain its functionality to potential users and contributors.