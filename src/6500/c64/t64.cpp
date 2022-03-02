#include <algorithm>
#include <bit>
#include <boost/endian/conversion.hpp>
#include <fstream>

#include "c64.h"
#include "t64.h"

static constexpr std::array<char, 4> Magic = { 'C', '6', '4', 'S' };

C64Tape::C64Tape(C64 &c64, const std::filesystem::path &path):
	Device(c64.GetBus()),
	m_c64(c64)
{
	std::ifstream tapeFile(path, std::ios::binary);
	tapeFile.read(std::bit_cast<char *>(&m_header), 64);

	if (std::equal(m_header.sig.begin(), m_header.sig.begin() + 4, Magic.begin()))
	{
		m_entries.reserve(m_header.maxDirEntries);

		for (size_t i = 0; i < m_header.usedEntries)
		{
			DirectoryEntry entry;

			tapeFile.read(std::bit_cast<char *>(&entry), 32);
			boost::endian::big_to_native_inplace(entry.fileOffset);

			// map the RAM
			if (entry.endAddr == 0)
			{
				if (i + 1 == m_header.usedEntries)
				{
					// assume the file goes to the end of the tape

					size_t pos = tapeFile.tellg();
					tapeFile.seekg(0, std::ios::end);
					size_t romSize = tapeFile.tellg() - entry.fileOffset;
					tapeFile.seekg(pos);

					AddRange(entry.startAddr, entry.startAddr + romSize);
				}
			}
			else
				AddRange(entry.startAddr, entry.endAddr);

			m_entries.push_back(entry);
		}

		for (auto &entry : m_entries)
		{
			if (entry.endAddr)
			{
				std::vector<uint8_t> data(entry.endAddr - entry.startAddr, 0);
				tapeFile.seekg(entry.fileOffset);
				tapeFile.read(std::bit_cast<char *>(&data[0]), data.size());
				buses[0]->Write(data, entry.startAddr);
			}
		}
	}
}
