set (MIPS_SOURCES
	devices.c
	devices.h
	isa.c
	isa.h
)

add_library (mips ${MIPS_SOURCES})
set_target_properties (mips PROPERTIES LINKER_LANGUAGE C)
