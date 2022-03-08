#include <algorithm>

#include "../../core/utility.h"
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

static const std::array<uint32_t, 64> PALETTE =
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
	Processor(nes.GetBus(), 0, 16383),
	m_cache(0),
	m_lastAddr(0),
	m_scanline(0),
	m_cycle(0),
	m_renderer(renderer),
	m_nes(nes),
	m_ppuBus(BusLE16(16384)),
	m_sprNameTbl(
	{
		Sprite(256, 240),
		Sprite(256, 240)
	}),
	m_sprPatTbl(
	{
		Sprite(128, 128),
		Sprite(128, 128)
	}),
	m_ramPalette(Sprite(16, 4))
{
	m_flags.frameDone = false;

	m_status.reg = 0;
	m_mask.reg = 0;
	m_ctrl.reg = 0;

	std::fill(m_palTbl.begin(), m_palTbl.end(), 0);
	std::fill(m_nameTbl.begin(), m_nameTbl.end(), 0);
	std::fill(m_patTbl.begin(), m_patTbl.end(), 0);

	if (renderer)
	{
		// Give the renderer a solid fill colour instead of copying what's underneath
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
	}

	AddRange(0, 16383, 1);
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
	uint8_t data = 0;

	switch (addr)
	{
		case 2:
			data = (m_status.reg & 0xE0) | (m_cache & 31);
			m_status.verticalBlank = false;
			m_flags.addrLatch = false;
			break;
		case 7:
			data = m_cache;
			m_cache = PPUReadByte(m_lastAddr);
			if (m_lastAddr > 0x3F00) data = m_cache;
			m_lastAddr++;
			break;
		default: ;
	}

	return data;
}

void PPU2C02::CPUWriteByte(uint16_t addr, uint8_t data)
{
	addr %= 8;

	switch (addr)
	{
		case 0: m_ctrl.reg = data; break;
		case 1: m_mask.reg = data; break;
		case 6:
			if (m_flags.addrLatch)
			{
				m_lastAddr = Hi16(m_lastAddr) | data;
				m_flags.addrLatch = true;
			}
			else
			{
				m_lastAddr = (m_lastAddr & 255) | (data << 8);
				m_flags.addrLatch = false;
			}

			break;
		case 7:
			PPUWriteByte(addr, data);
			m_lastAddr++;
			break;
		default: ;
	}
}

BusLE16 * PPU2C02::GetBus()
{
	return &m_ppuBus;
}

Sprite & PPU2C02::GetSprite(uint8_t i, uint8_t palette)
{
	for (uint16_t y = 0; y < 16; y++)
		for (uint16_t x = 0; x < 16; x++)
		{
			uint16_t offset = y * 256 + x * 16;

			for (uint16_t row = 0; row < 8; row++)
			{
				// A full pattern table is 4 KB, and there's 2 of them: foreground + background
				// Each row is 1 byte. We want to grab the least significant bit and corresponding
				// most significant bit, for the foreground and background respectively.
				uint8_t lsb = PPUReadByte(i * 4096 + offset + row);
				uint8_t msb = PPUReadByte(i * 4096 + offset + row + 8);

				for (uint16_t col = 0; col < 8; col++)
				{
					// We only want to combine the LSB of each value to obtain our pixel colour.
					// We also need to shift LSB and MSB by 1 each calculation, so the next bit is the LSB.
					uint8_t pixel = (lsb & 1) + (msb & 1);
					lsb >>= 1;
					msb >>= 1;

					// LSB refers to rightmost pixel, so subtract column from 7, since we're drawing from top-left.
					m_sprPatTbl[i].Set(x * 8 + (7 - col), y * 8 + row, ReadRAMPaletteColor(palette, pixel));
				}
			}
		}

	return m_sprPatTbl[i];
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
	uint8_t data = 0;

	if (addr >= 0 && addr <= 8191)
		// MSB + remaining bits
		data = m_patTbl[((addr & 4096) >> 12) * 64 + (addr & 4095)];
	else if (addr > 0x3F00 && addr <= 16383)
	{
		addr %= 31;
		if (addr == 16) addr = 0;
		if (addr == 20) addr = 4;
		if (addr == 24) addr = 8;
		if (addr == 28) addr = 12;
		data = m_palTbl[addr];
	}

	return data;
}

void PPU2C02::PPUWriteByte(uint16_t addr, uint8_t data)
{
	if (addr >= 0 && addr <= 8191)
		// MSB + remaining bits
		m_patTbl[((addr & 4096) >> 12) * 64 + (addr & 4095)] = data;
	else if (addr > 0x3F00 && addr <= 16383)
	{
		addr %= 31;
		if (addr == 16) addr = 0;
		if (addr == 20) addr = 4;
		if (addr == 24) addr = 8;
		if (addr == 28) addr = 12;
		m_palTbl[addr] = data;
	}
}

SDL_Color PPU2C02::ReadRAMPaletteColor(uint8_t palette, uint8_t pixel)
{
	// Palettes are 4 bytes, so we need to multiply ID by 4
	return m_ramPalette[PPUReadByte(0x3F00 + (palette << 2) + pixel)];
}
