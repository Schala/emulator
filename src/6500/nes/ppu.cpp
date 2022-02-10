#include "nes.h"

static constexpr SDL_Color ColorFromU32(uint32_t u)
{
	return
	{
		static_cast<uint8_t>((u & 0xFF000000) >> 24),
		static_cast<uint8_t>((u & 0xFF0000) >> 16),
		static_cast<uint8_t>((u & 0xFF00) >> 8),
		static_cast<uint8_t>(u & 255)
	};
}

const std::array<uint32_t, 64> PPU2C02::Palette =
{
	0x545454FF,
	0x1E74FF,
	0x81090FF,
	0x300088FF,
	0x440064FF,
	0x5C0030FF,
	0x540400FF,
	0x3C1800FF,
	0x202A00FF,
	0x83A00FF,
	0x4000FF,
	0x3C00FF,
	0x323CFF,
	255,
	255,
	255,

	0x989698FF,
	0x84CC4FF,
	0x3032ECFF,
	0x5C1EE4FF,
	0x8814B0FF,
	0xA01464FF,
	0x982220FF,
	0x783C00FF,
	0x545A00FF,
	0x287200FF,
	0x87C00FF,
	0x7628FF,
	0x6678FF,
	255,
	255,
	255,

	0xECEEECFF,
	0x4C9AECFF,
	0x787CECFF,
	0xB062ECFF,
	0xE454ECFF,
	0xEC58B4FF,
	0xEC6A64FF,
	0xD48820FF,
	0xA0AA00FF,
	0x74C400FF,
	0x4CD020FF,
	0x38CC6CFF,
	0x38B4CCFF,
	0x383838FF,
	255,
	255,

	0xECEEECFF,
	0xA8CCECFF,
	0xBCBCECFF,
	0xD4B2ECFF,
	0xECAEECFF,
	0xECAED4FF,
	0xECB4B0FF,
	0xE4C490FF,
	0xCCD278FF,
	0xB4DE78FF,
	0xA8E290FF,
	0x98E2B4FF,
	0xA0D6E4FF,
	0xA0A2A0FF,
	255,
	255
};

PPU2C02::PPU2C02(NES &nes, SDL_Renderer *renderer):
	Processor(nes.GetBus(), 0, 0x3FFF),
	m_scanline(0),
	m_cycle(0),
	m_renderer(renderer),
	m_nes(nes),
	m_ppuBus(Bus6500(0x4000)),
	m_nameTbl(
	{
		Sprite(32, 32),
		Sprite(32, 32)
	}),
	m_patTbl(
	{
		Sprite(64, 64),
		Sprite(64, 64)
	}),
	m_ramPalette(Sprite(16, 4))
{
	m_flags.frameDone = false;

	if (renderer)
	{
		// Give the renderer a solid fill colour instead of copying what's underneath
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
	}

	AddRange(0, 0x3FFF, 1);
	m_ppuBus.Add(this);
}

PPU2C02::~PPU2C02()
{
}

void PPU2C02::Clock()
{
	if (!m_renderer) return;

	if (++m_cycle >= 341)
	{
		m_cycle = 0;

		if (++m_scanline >= 261)
		{
			m_scanline = -1;
			m_flags.frameDone = true;
		}
	}
}

uint8_t PPU2C02::CPUReadByte(uint16_t addr)
{
	addr %= 8;
	return ReadByte(addr);
}

void PPU2C02::CPUWriteByte(uint16_t addr, uint8_t data)
{
	addr %= 8;
	WriteByte(addr, data);
}

Bus6500 * PPU2C02::GetBus()
{
	return &m_ppuBus;
}

bool PPU2C02::IsFrameDone() const
{
	return m_flags.frameDone;
}

void PPU2C02::NextFrame()
{
	m_flags.frameDone = false;
}

/*void PPU2C02::NoiseTest()
{
	SDL_Color c = ColorFromU32(Palette[m_rng(m_mt) ? 63 : 48]);
	SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(m_renderer, m_cycle - 1, m_scanline);
}*/

uint8_t PPU2C02::PPUReadByte(uint16_t addr)
{
	addr %= 8;
	return ReadByte(addr, 1);
}

void PPU2C02::PPUWriteByte(uint16_t addr, uint8_t data)
{
	addr %= 8;
	WriteByte(addr, data, 1);
}

SDL_Color PPU2C02::ReadRAMPaletteColor(uint8_t palette, uint8_t pixel)
{
	return m_ramPalette[PPUReadByte(0x3F00 + (palette << 2) + pixel)];
}
