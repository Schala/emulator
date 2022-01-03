set (M68K_SOURCES
	devices.c
	devices.h
	isa.c
	isa.h
)

add_library (m68k ${M68K_SOURCES})
set_target_properties (m68k PROPERTIES LINKER_LANGUAGE C)
