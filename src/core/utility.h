#ifndef _CORE_UTILITY_H
#define _CORE_UTILITY_H

bool IsBinary(char c)
{
	if (c == '0' || c == '1')
		return true;
	return false;
}

bool IsNewLine(char c)
{
	switch (c)
	{
		case '\n': [[fallthrough]]
		case '\r':
			return true;
		default:
			return false;
	}
}

#endif // _CORE_UTILITY_H
