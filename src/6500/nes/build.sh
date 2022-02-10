g++ -std=gnu++20 -g -o emu6500 main.cpp ../../core/engine.cpp ../../core/devices.cpp \
../devices.cpp mapper.cpp nrom.cpp ppu.cpp rom.cpp nes.cpp `sdl2-config --cflags --libs` -lfmt
