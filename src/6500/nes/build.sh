g++ -std=gnu++20 -g -o emu6500 \
main.cpp \
../../core/engine.cpp \
../../core/engine_core.cpp \
../../core/devices.cpp \
../../generic/bus_le16.cpp \
../cpu.cpp \
mapper.cpp \
cheater.cpp \
mappers/nrom.cpp \
ppu.cpp \
rom.cpp \
nes.cpp \
`sdl2-config --cflags --libs` -lfmt
