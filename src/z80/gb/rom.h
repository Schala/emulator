#ifndef _GB_ROM_H
#define _GB_ROM_H

#include <array>
#include <filesystem>

class GBROM : public Device
{
public:
	uint16_t Hash() const;
private:
	struct Header
	{
		std::array<uint8_t, 4> entry;
		std::array<uint8_t, 48> logo;
		std::array<char, 15> title;
		std::array<char, 4> manufacturer;
		uint8_t cgbFlag;
		std::array<uint8_t, 2> licensee;
		uint8_t sgbFlag;
		uint8_t type;
		uint8_t romSize;
		uint8_t ramSize;
		bool notJapanese;
		uint8_t oldLicensee;
		uint8_t version;
		uint8_t headerChecksum;
		uint16_t globalChecksum;
	} m_header;
};

#endif // _GB_ROM_H
