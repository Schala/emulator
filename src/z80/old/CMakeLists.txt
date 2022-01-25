set (Z80_SOURCES
	dev.c
	dev.h
	isa.c
	isa.h
)

add_library (z80 ${Z80_SOURCES})
set_target_properties (z80 PROPERTIES LINKER_LANGUAGE C)
