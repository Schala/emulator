#include "debugger.h"

Debugger6500::Debugger6500(MOS6500 &cpu):
	cpu(cpu)
{
}

void Debugger6500::AddWatch(uint16_t addr)
{
	watches[addr] = std::vector<uint8_t>();
}

void Debugger6500::DeleteWatch(uint16_t addr)
{
	watches.erase(addr);
}

void WatchAccumulator(bool);
void WatchCache(bool);
void WatchCounter(bool);
void WatchForBreak(bool);
void WatchForCarry(bool);
void WatchForDisableInterrupts(bool);
void WatchForNegative(bool);
void WatchForOverflow(bool);
void WatchForZero(bool);
void WatchStackPtr(bool);
void WatchX(bool);
void WatchY(bool);
