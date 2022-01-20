gcc -g -o emu6502 main.c util.c eng.c 6502/bus.c 6502/cpu.c 6502/isa.c 6502/nes/rom.c 6502/nes/ppu.c 6502/nes/nes.c -lSDL2 -lSDL2main
