set (MIPS_SOURCES
	dev.c
	dev.h
	isa.c
	isa.h
)

add_library (mips ${MIPS_SOURCES})
set_target_properties (mips PROPERTIES LINKER_LANGUAGE C)
