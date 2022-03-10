#ifndef _6500_DEBUGGER_H
#define _6500_DEBUGGER_H

#include <map>
#include "cpu.h"

class Debugger6500
{
public:
	Debugger6500(MOS6500 &);
	void AddAccumulatorValue(uint8_t);
	void AddCacheValue(uint8_t);
	void AddCounterValue(uint16_t);
	void AddStackPtrValue(uint8_t);
	void AddWatch(uint16_t);
	void AddWatchValue(uint8_t);
	void AddXValue(uint8_t);
	void AddYValue(uint8_t);
	void Clock();
	void DeleteAccumulatorValue(uint8_t);
	void DeleteWatch(uint16_t);
	void DeleteWatchValue(uint8_t);
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
protected:
	union
	{
		struct
		{
			bool
				a : 1,
				b : 1,
				c : 1,
				cache : 1,
				i : 1,
				n : 1,
				pc : 1,
				s : 1,
				v : 1,
				x : 1,
				y : 1,
				z : 1;
		} w;

		uint16_t watch;
	};

	MOS6500 &cpu;
	std::map<uint16_t, std::vector<uint8_t>> watches;
};

#endif // _6500_DEBUGGER_H
