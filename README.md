# Quake for embedded devices

This project is yet another WinQuake port for embedded devices, primarily for RISC-V devices.

![QuakEMBD on Action](https://i.imgur.com/wctRYIJ.gif)

Based on original [Quake GPL source](https://github.com/id-Software/Quake).

## How to build

Use CMake with [GNU toolchain for RISC-V](https://github.com/riscv-collab/riscv-gnu-toolchain) installed.

Build Instruction:
```shell
git clone https://github.com/sysprog21/quake-embedded && cd quake-embedded
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../port/boards/rv32emu/toolchain.cmake \
      -DCMAKE_BUILD_TYPE=RELEASE -DBOARD_NAME=rv32emu ..
make
```
