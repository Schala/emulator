g++ -std=gnu++20 -g -o emu6502 main.cpp core/engine.cpp core/devices.cpp 6502/devices.cpp \
6502/nes_mappers.cpp 6502/nes.cpp `sdl2-config --cflags --libs` -lfmt
