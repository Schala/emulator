x86_64-w64-mingw32-g++ -std=gnu++20 -o lexer6500.exe \
main.cpp \
lexer.cpp \
../../generic/scanner.cpp \
-static-libgcc -static-libstdc++
x86_64-w64-mingw32-strip lexer6500.exe
