# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -fdiagnostics-color=always -Wall -g -Wno-deprecated
INCLUDES = -Iinclude
LIBS = -Llib -lglfw.3.4 -lassimp.5.4.3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation

# Source files and output
SRC = $(wildcard *.cpp glad.c)
EXE = app

# Build target
all: $(EXE)

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) $^ -o $@

.PHONY: format
format:
	find . -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -exec clang-format -i {} +

# Clean target
clean:
	rm -f $(EXE)

