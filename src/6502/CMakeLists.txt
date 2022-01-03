set (MOS6502_SOURCES
	devices.c
	devices.h
	isa.c
	isa.h
)

add_library (mos6502 ${MOS6502_SOURCES})
set_target_properties (mos6502 PROPERTIES LINKER_LANGUAGE C)
