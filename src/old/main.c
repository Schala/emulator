#include <ctype.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#include "6502/nes/ppu.h"
#include "6502/nes/rom.h"
#include "eng.h"

void noise_test(NES *nes)
{
	while (1)
	{
		if (!nes->ppu->flags.frame_complete)
		{
			nes_clock(nes);
			nes_ppu_noise_test(nes);
		}
		else break;
	}

	nes->ppu->flags.frame_complete = 0;
}

int main(int argc, char **argv)
{
	SDL_Window *win = SDL_CreateWindow("6502", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480, SDL_WINDOW_RESIZABLE);
	NES *nes = nes_alloc(win);
	ENG *eng = eng_alloc();
	nes_rom_alloc(nes, argv[1]);
	int r = 0;
	int w, h;

	if ((r = SDL_Init(SDL_INIT_VIDEO)) != 0)
		return r;

	printf("Ready\n");

	/*if (nes->rom)
	{
		nes_print_rom_info(nes);
		printf("\n");
	}*/

	SDL_Event evt;
	SDL_GetWindowSize(win, &w, &h);
	SDL_RenderSetScale(nes->ppu->renderer, w / NES_SCREEN_WIDTH, h / NES_SCREEN_HEIGHT);

	float t = 0.0f;

	while (SDL_WaitEvent(&evt))
	{
		if (evt.type == SDL_WINDOWEVENT)
		{
			switch (evt.window.event)
			{
				case SDL_WINDOWEVENT_CLOSE:
					goto exitloop;
					break;

				case SDL_WINDOWEVENT_RESIZED:
					SDL_GetWindowSize(win, &w, &h);
					SDL_RenderSetScale(nes->ppu->renderer, w / NES_SCREEN_WIDTH, h / NES_SCREEN_HEIGHT);
					break;
				default: ;
			}
		}

		if (t > 0.0f)
			t -= eng_clock(eng);
		else
		{
			t = fabsf(t) + (1.0 / NTSC_FPS) - eng_clock(eng);
			printf("%f\n", t);
			noise_test(nes);
			nes_ppu_update(nes);
		}
			/*char c = 0;
			scanf("%c", &c);

			switch (toupper(c))
			{
				case 'R':
					running = 1;

				case 'A':
					printf("\n");
					disasm6502_print(nes->cpu->disasm);
					printf("\n");
					break;
				case 'B':
				{
					DISASM_6502 *disasm = disasm6502(nes->rom->prg, nes->map->prg_size *
						nes->rom->header.prg_pages);
					disasm6502_print(disasm);
					disasm6502_free(disasm);
					printf("\n");
				}
					break;
				case 'C':
					nes_print_chr(nes);
					break;
				case 'D':
					printf("\n");
					//bus6502_ram_dump(bus);
					bus6502_print_ram(nes->bus);
					printf("\n");
					break;
				case 'F':
					printf("\n");
					cpu6502_print_disasm(nes->cpu, 5);
					printf("\n");
					break;
				case 'P':
					nes_print_prg(nes);
					break;
				case 'Q':
					goto exit_loop;
					break;
				case 'R':
					printf("\n");
					cpu6502_print_regs(nes->cpu);
					printf("\n");
					break;
				case 'S':
					disasm6502(nes->rom->chr, nes->map->chr_size * nes->rom->header.chr_pages);
					printf("\n");
					break;
				default:
					do cpu6502_clock(nes->cpu); while (nes->cpu->cycles != 0);
					printf("Counter @ %04X\n", nes->cpu->regs.pc);*/
	}
exitloop:
	nes_free(nes);
	eng_free(eng);
	SDL_DestroyWindow(win);

	return 0;
}
