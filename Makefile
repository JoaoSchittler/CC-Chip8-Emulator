CXX = g++
CXXFLAGS = -g -std=c++11 -Wall 
LDFLAGS = -lallegro -lallegro_main \
    -lallegro_color -lallegro_font -lallegro_primitives -lallegro_image -lallegro_audio -lallegro_acodec

all: main



chip8.o: chip8.cpp chip8.hpp instructions.hpp
main.o: main.cpp chip8.hpp
main: main.o chip8.o
	$(CXX) $(CXXFLAGS) -o $@  $^ $(LDFLAGS)

clean:
	rm -f main *.o
