#include <fmt/core.h> // todo: <format>

#include "nes.h"

static constexpr uint16_t Hi16(uint16_t value)
{
	return value & 0xFF00;
}

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

static const std::array<SDL_Color, 64> PALETTE =
{
	ColorFromU32(0x545454FF),
	ColorFromU32(0x1E74FF),
	ColorFromU32(0x81090FF),
	ColorFromU32(0x300088FF),
	ColorFromU32(0x440064FF),
	ColorFromU32(0x5C0030FF),
	ColorFromU32(0x540400FF),
	ColorFromU32(0x3C1800FF),
	ColorFromU32(0x202A00FF),
	ColorFromU32(0x83A00FF),
	ColorFromU32(0x4000FF),
	ColorFromU32(0x3C00FF),
	ColorFromU32(0x323CFF),
	ColorFromU32(255),
	ColorFromU32(255),
	ColorFromU32(255),

	ColorFromU32(0x989698FF),
	ColorFromU32(0x84CC4FF),
	ColorFromU32(0x3032ECFF),
	ColorFromU32(0x5C1EE4FF),
	ColorFromU32(0x8814B0FF),
	ColorFromU32(0xA01464FF),
	ColorFromU32(0x982220FF),
	ColorFromU32(0x783C00FF),
	ColorFromU32(0x545A00FF),
	ColorFromU32(0x287200FF),
	ColorFromU32(0x87C00FF),
	ColorFromU32(0x7628FF),
	ColorFromU32(0x6678FF),
	ColorFromU32(255),
	ColorFromU32(255),
	ColorFromU32(255),

	ColorFromU32(0xECEEECFF),
	ColorFromU32(0x4C9AECFF),
	ColorFromU32(0x787CECFF),
	ColorFromU32(0xB062ECFF),
	ColorFromU32(0xE454ECFF),
	ColorFromU32(0xEC58B4FF),
	ColorFromU32(0xEC6A64FF),
	ColorFromU32(0xD48820FF),
	ColorFromU32(0xA0AA00FF),
	ColorFromU32(0x74C400FF),
	ColorFromU32(0x4CD020FF),
	ColorFromU32(0x38CC6CFF),
	ColorFromU32(0x38B4CCFF),
	ColorFromU32(0x383838FF),
	ColorFromU32(255),
	ColorFromU32(255),

	ColorFromU32(0xECEEECFF),
	ColorFromU32(0xA8CCECFF),
	ColorFromU32(0xBCBCECFF),
	ColorFromU32(0xD4B2ECFF),
	ColorFromU32(0xECAEECFF),
	ColorFromU32(0xECAED4FF),
	ColorFromU32(0xECB4B0FF),
	ColorFromU32(0xE4C490FF),
	ColorFromU32(0xCCD278FF),
	ColorFromU32(0xB4DE78FF),
	ColorFromU32(0xA8E290FF),
	ColorFromU32(0x98E2B4FF),
	ColorFromU32(0xA0D6E4FF),
	ColorFromU32(0xA0A2A0FF),
	ColorFromU32(255),
	ColorFromU32(255)
};

PPU2C02::PPU2C02(NES &nes, SDL_Renderer *renderer):
	Processor(nes.GetBus(), 8192, 16383),
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
	})
{
	if (renderer)
	{
		// Give the renderer a solid fill colour instead of copying what's underneath
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
	}

	AddRange(0, 16383, 1);
	m_ppuBus.Add(this);
	buses.push_back(&m_ppuBus);
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
			//m_status.p.verticalBlank = true;
			data = (m_status.reg & 224) | (m_cache & 31);
			m_status.p.verticalBlank = false;
			m_flags.addressLatch = false;
			break;
		case 7:
			data = m_cache;
			m_cache = PPUReadByte(m_lastAddr);
			if (m_lastAddr > 16128) data = m_cache;
			m_lastAddr++;
			break;
		default: ;
	}

	UpdateRegisters();
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
			if (m_flags.addressLatch)
			{
				m_lastAddr = Hi16(m_lastAddr) | data;
				m_flags.addressLatch = true;
			}
			else
			{
				m_lastAddr = (m_lastAddr & 255) | (data << 8);
				m_flags.addressLatch = false;
			}

			break;
		case 7:
			PPUWriteByte(addr, data);
			m_lastAddr++;
			break;
		default: ;
	}

	UpdateRegisters();
}

std::string PPU2C02::FrameInfo()
{
	//SDL_Color c = PALETTE[(m_scanline, m_cycle);
	std::string s = fmt::format("X, Y: {},{}\n", m_scanline, m_cycle);// (#{:06X})\n", m_scanline, m_cycle, (c.r << 16) | (c.g << 8) | c.b);

	s += fmt::format("Last address: ${:04X}\n", m_lastAddr);
	s += fmt::format("Last buffered byte: $#{:02X}\n\n", m_cache);

	s += "Control flags:\n";
	if (m_ctrl.p.nameTableX) s += "\t- name table X\n";
	if (m_ctrl.p.nameTableY) s += "\t- name table Y\n";
	if (m_ctrl.p.increment) s += "\t- increment\n";
	if (m_ctrl.p.patternFG) s += "\t- sprite pattern\n";
	if (m_ctrl.p.patternBG) s += "\t- background pattern\n";
	if (m_ctrl.p.spriteSize) s += "\t- sprite size\n";
	if (m_ctrl.p.nmi) s += "\t- non-maskable interrupts\n";

	s += "\nMask flags:\n";
	if (m_mask.p.grayscale) s += "\t- grayscale\n";
	if (m_mask.p.renderBGLeft) s += "\t- render background left\n";
	if (m_mask.p.renderFGLeft) s += "\t- render sprite left\n";
	if (m_mask.p.renderBG) s += "\t- render background\n";
	if (m_mask.p.renderFG) s += "\t- render sprites\n";
	if (m_mask.p.enhanceRed) s += "\t- enhance red\n";
	if (m_mask.p.enhanceGreen) s += "\t- enhance green\n";
	if (m_mask.p.enhanceBlue) s += "\t- enhance blue\n";

	s += "\nStatus flags:\n";
	if (m_status.p.spriteOverflow) s += "\t- sprite overflow\n";
	if (m_status.p.sprite0Hit) s += "\t- sprite zero hit\n";
	if (m_status.p.verticalBlank) s += "\t- vartical blank\n";

	return std::move(s);
}

BusLE16 * PPU2C02::GetBus()
{
	return &m_ppuBus;
}

Sprite & PPU2C02::GetPatternTable(uint8_t i, uint8_t palette)
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
					m_sprPatTbl.at(i).Set(x * 8 + (7 - col), y * 8 + row, ReadRAMPaletteColor(palette, pixel));
				}
			}
		}

	return m_sprPatTbl.at(i);
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
	SDL_Color c = ColorFromU32(PALETTE[m_rng(m_mt) ? 63 : 48]);
	SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(m_renderer, m_cycle - 1, m_scanline);
}*/

uint8_t PPU2C02::PPUReadByte(uint16_t addr)
{
	if (addr >= 0 && addr <= 8191)
		// MSB + remaining bits
		ReadByte(((addr & 4095) >> 12) * 64 + (addr & 4095), 1);
	else if (addr > 16128 && addr <= 16383)
	{
		addr %= 31;
		if (addr == 16) addr = 0;
		if (addr == 20) addr = 4;
		if (addr == 24) addr = 8;
		if (addr == 28) addr = 12;
		ReadByte(addr, 1);
	}

	return 0;
}

void PPU2C02::PPUWriteByte(uint16_t addr, uint8_t data)
{
	if (addr >= 0 && addr <= 8191)
		// MSB + remaining bits
		WriteByte(((addr & 4095) >> 12) * 64 + (addr & 4095), data, 1);
	else if (addr > 16128 && addr <= 16383)
	{
		addr %= 31;
		if (addr == 16) addr = 0;
		if (addr == 20) addr = 4;
		if (addr == 24) addr = 8;
		if (addr == 28) addr = 12;
		WriteByte(addr, data, 1);
	}
}

SDL_Color PPU2C02::ReadRAMPaletteColor(uint8_t palette, uint8_t pixel)
{
	// Palettes are 4 bytes, so we need to multiply ID by 4
	return PALETTE.at(PPUReadByte(16128 + (palette << 2) + pixel) & 63);
}

void PPU2C02::Reset()
{
	m_flags.frameDone = false;
	m_flags.addressLatch = false;
	m_status.reg = 0;
	m_mask.reg = 0;
	m_ctrl.reg = 0;
	m_cache = 0;
	m_lastAddr = 0;
	m_scanline = 0;
	m_cycle = 0;
}

void PPU2C02::UpdateRegisters()
{
	WriteByte(8192, m_ctrl.reg);
	WriteByte(8193, m_mask.reg);
	WriteByte(8194, m_status.reg);
}

