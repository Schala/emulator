#ifndef _EMU_UTIL_H
#define _EMU_UTIL_H

// High bytes of a 16-bit value
#define HI16(x) ((x) & 0xFF00)

// reverse bytes of a 16 bit value
#define SWAP16(x) ((((x) & 0xFF00) >> 8) | (((x) & 255) << 8))

// reverse bytes of a 32 bit value
#define SWAP32(x) \
	((((x) & 0xFF000000) >> 24) | \
	(((x) & 0xFF0000) >> 8) | \
	(((x) & 0xFF00) << 8) | \
	(((x) & 255) << 24))

#endif // _EMU_UTIL_H
