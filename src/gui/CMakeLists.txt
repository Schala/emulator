set (APP_SOURCES
	app.cpp
	app.h
	main_win.cpp
	main_win.h
	memview.cpp
	memview.h
)

add_executable (EmuGUI ${APP_SOURCES})
add_dependencies (EmuGUI mos6502)
target_include_directories (EmuGUI ${wxWidgets_INCLUDE_DIRS} ${SFML_INCLUDE_DIRS})
target_link_libraries (EmuGUI ${wxWidgets_LIBRARIES} ${SFML_LIBRARIES})
