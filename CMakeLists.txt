project (EmuGUI)
cmake_minimum_required (VERSION 3.18)

find_package (wxWidgets REQUIRED base core)
find_package (SFML 2.5 COMPONENTS audio graphics system window REQUIRED)

add_subdirectory (src)
