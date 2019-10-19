CXX = gcc
CXXFLAGS = -o
LDFLAGS = -lallegro -lallegro_main \
    -lallegro_color -lallegro_font -lallegro_primitives -lallegro_image -lallegro_audio -lallegro_acodec

all: $(TARGET)

screen.o: screen.c screen.h
chip8.o: chip8.c chip8.h 
main.o: main.c chip8.h screen.h
main: main.o chip8.o screen.o
	$(CXX) $(CXXFLAGS) $(TARGET) $(LDFLAGS)

clean:
	rm -f main *.o
