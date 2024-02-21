#pragma once

/// Central processing unit
template <std::endian Endian, size_t PtrSize>
class TCentralProcessor<Endian, PtrSize> : public TProcessor<Endian>
{
public:
	typedef TProcess<Endian, PtrSize> ProcT;
	typedef std::shared_ptr<ProcT> ProcP;

	TCentralProcessor(BusP bus, size_t startAddr, size_t endAddr):
		TProcessor<Endian>(bus, startAddr, endAddr)
	{
	}
protected:
};
