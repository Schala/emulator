#ifndef _C64_T64_H
#define _C64_T64_H

#include <array>
#include <filesystem>

#include "../../core/devices.h"
#include "fwd.h"

class C64Tape : public Device
{
public:
	C64Tape(C64 &, const std::filesystem::path &);
private:
	static constexpr std::array<char, 4> Magic = { 'C', '6', '4', 'S' };

	struct DirectoryEntry
	{
		uint8_t c64sFileType; // 0 = free (usually), 1 = normal, 3 = uncompressed mem snapshot,
							  // 2-255 = mem snapshots
		uint8_t fileType; // 0x82 = PRG, 0x81 = SEQ, etc.
		uint16_t startAddr;
		uint16_t endAddr; // 0 if snapshot
		uint16_t _reserved06;
		uint32_t fileOffset; // big endian
		uint32_t _reserved0A;
		std::array<char, 16> name; // padded with space (0x20)
	};

	struct Header
	{
		std::array<char, 32> sig;
		uint16_t version;
		uint16_t maxDirEntries;
		uint16_t usedEntries;
		uint16_t _reserved26;
		std::array<char, 24> name; // padded with space (0x20)
	} m_header;

	C64 &m_c64;
	std::vector<DirectoryEntry> m_entries;
};

#endif // _C64_T64_H
