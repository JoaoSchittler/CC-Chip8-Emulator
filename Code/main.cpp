#include "Chip8.hpp"

int main()
{
	chip8::Chip8 processor;
	processor.init();
	processor.loadGame("ROMS/KALEID");
	
}
