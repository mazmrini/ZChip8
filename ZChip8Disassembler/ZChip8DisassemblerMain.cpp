#include <fstream>
#include <algorithm>
#include <cstdio>

#include "ZChip8Disassembler.hpp"

using namespace std;

int main(int argc, char * argv[]) 
{

	// read the first argument as a binary file
	std::ifstream chip8File(argv[1], std::ios::binary);
	
	if (!chip8File.is_open())
	{
		printf("Error : Could not open %s\n", argv[1]);
		return 1;
	}
	
	/*
	// GET FILE SIZE
	fseek(chip8File, 0L, SEEK_END); // move the pointer to the end of the file
	int fsize = ftell(chip8File); // number of bytes from the beginning of the file to the pointer position in the stream = file size (since we used rb to open)
	rewind(chip8File); // return the pointer to the beginning of the file
	
	unsigned char *buffer = malloc(fsize + 0x200); // allocates memory for the file's size + 0x200, since chip8 programs starts at 0x200
	// file is read at buffer location + 0x200 meaning the 0x200 bits of the buffer will be empty
	// this is for consistency with chip8 way of working
	fread(buffer+0x200, 1, fsize, chip8File);
	
	// close file f
	fclose(chip8File);
	*/
	
	char *buffer = Disassembler::readFileBytes(chip8File);
	
	printf("%-6s    %-6s    %-16s : %s\n", "ADDR", "OP", "INSTRUCTION", "DETAILS");
	printf("------    ------    ----------------   ------------------------------------------");
	
	// program counter for the code and opcode
	Disassembler::u16int pc = 0x200;
	
	while (pc < sizeof(buffer)) 
	{
		Disassembler::opcode = (buffer[pc] << 8) | (buffer[pc + 1]);
		
		Disassembler::Vx = (Disassembler::opcode & 0x0F00) >> 8;
		Disassembler::Vy = (Disassembler::opcode & 0x00F0) >> 4;
		Disassembler::kk = Disassembler::opcode & 0x00FF;
		Disassembler::nnn = Disassembler::opcode & 0x0FFF;
		
		printf("0x%04X    0x%04X    ", pc, Disassembler::opcode);
		
		Disassembler::Chip8Instructions[(Disassembler::opcode & 0xF000) >> 12]();
		
		pc += 2;
		
		printf("\n");
		
	}
	
	//free (buffer);
	return 0;
}
