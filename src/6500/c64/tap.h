#ifndef _C64_TAP_H
#define _C64_TAP_H

#include <array>
#include <filesystem>

#include "../../core/devices.h"
#include "fwd.h"

class C64RawTape : public Device
{
public:
	C64RawTape(C64 &, const std::filesystem::path &);
	double Pulse();
private:
	struct Header
	{
		std::array<char, 12> sig;
		uint8_t version;
		uint32_t _reserved0D : 24;
		uint32_t dataSize;
	} m_header;

	C64 &m_c64;
	std::vector<uint8_t>::iterator m_it;
	std::vector<uint8_t> m_data;
};

#endif // _C64_TAP_H
