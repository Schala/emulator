#include <algorithm>
#include <bit>
#include <fstream>

#include "c64.h"
#include "tap.h"

C64RawTape::C64RawTape(C64 &c64, const std::filesystem::path &path):
	Device(c64.GetBus()),
	m_c64(c64),
	m_lastPulse(0.0)
{
	std::ifstream tapeFile(path, std::ios::binary);
	tapeFile.read(std::bit_cast<char *>(&m_header), 20);

	if (std::equal(m_header.sig.begin(), m_header.sig.begin() + 4, Magic.begin()))
	{
		m_data.resize(m_header.dataSize, 0);
		tapeFile.read(std::bit_cast<char *>(&m_data[0]), m_header.dataSize);
	}

	m_it = m_data.begin();
}

double C64RawTape::Pulse()
{
	if (m_it == m_data.end()) return 0.0;

	// Predominantly PAL, so we go with that format's cycle amount.
	// NTSC is almost the same, so shouldn't have issues.
	return static_cast<double>(*(m_it++) * 8) / 985248.0;
}
