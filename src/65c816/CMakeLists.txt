set (WDC65C816_SOURCES
	dev.c
	dev.h
	isa.c
	isa.h
)

add_library (wdc65c816 ${WDC65C816_SOURCES})
set_target_properties (wdc65c816 PROPERTIES LINKER_LANGUAGE C)
