#ifndef _NES_CHEATER_H
#define _NES_CHEATER_H

#include <filesystem>
#include <map>
#include <utility>

#include "rom.h"

class NESCheater : public Device
{
public:
	NESCheater(BusLE16 *, NESROM *);
	void AddCompare(uint16_t, uint8_t, uint8_t);
	void AddGameGenieCode(const std::string &);
	void AddIntercept(uint16_t, uint8_t);

	// Write to CPU bus
	void CPUWriteByte(uint16_t, uint8_t);

	void Save(const std::filesystem::path &) const;
	void UpdateInterceptRAM();
private:
	NESROM *m_rom;
	std::map<uint16_t, uint8_t> m_intercepts;
	std::map<uint16_t, std::pair<uint8_t, uint8_t>> m_compares;
};

#endif // _NES_CHEATER_H
