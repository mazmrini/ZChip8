// TESTS
#include "Chip8.h";
using std;

void Chip8::Chip8() {
	// program starts at 0x200 in memory
	regPc = 0x200;
	
	// clear address register and stack pointer
	I = 0;
	sp = 0;
	
	// clear stack
	stack = { };
	
	// clear display
	gfx = { };
	
	
	// clear registers V0-Vf
	regV = { };
	
	// clear memory
	memory = { };
	
	// load fontset
	u8int chip8FontSet[] =
		{ 
		  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		  0x20, 0x60, 0x20, 0x20, 0x70, // 1
		  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
		};
	
	std::copy(std::begin(chip8FontSet), std::end(chip8FontSet), memory);
	
	// reset timers
	
	// LOAD PROGRAM INTO MEMORY

} // end of constructor

void Chip8::emulateCycle() {
	
	// fetch opcode
	u16int opcode = memory[regPc] << 8 | memory[regPc + 1];
	
	// decode opcode
	decodeOpcode(opcode);
	
} // fin du emulateCycle

void Chip8::decodeOpcode(u16int opcode) {
	
	// REFER TO http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00EE
	
				
	// only possibilities are X and Y
	u8int *Vx = &regV[opcode & 0x0F00 >> 8];
	u8int *Vy = &regV[opcode & 0x00F0 >> 4];
	u8int *Vf = &regV[0xF];
	
	// decode opcode
	switch (opcode & 0xF000) 
	{
		case 0x0000:
			switch (opcode & 0x00FF) 
			{
				// 0x00E0 : Clears the screen.
				case 0x00E0:
					gfx = { };
					pc += 2;
					break;
					
				// 0x00EE : Returns from a subroutine
				case 0x00EE:
					sp--;
					regPc = stack[sp];
					break;
				
				default: 
					unknownOpcode(opcode);
			}
			break;
		
		case 0x1000:
			// 0x1NNN : Jump to location nnn.
			regPc = opcode & 0x0FFF;
			break;
		
		case 0x2000:
			// 0x2NNN : Call subroutine at NNN.
			stack[sp] = regPc;
			sp++;
			
			regPc = opcode & 0x0FFF;
			break;
		
		case 0x3000:
			// 0x3XNN : Skip next instruction if Vx = NN.
			pc += (*Vx == (opcode & 0x00FF)) ? 4 : 2;
			break;
		
		case 0x4000:
			// 0x4XNN : Skip next instruction if Vx != NN.
			pc += (*Vx != (opcode & 0x00FF)) ? 4 : 2;
			break;
			
		case 0x5000:
			// 0x5XY0 : Skip next instruction if Vx = kk.
			pc += (*Vx == *Vy) ? 4 : 2;
			break;
		
		case 0x6000:
			// 0x6XNN : Set Vx = NN;
			*Vx = opcode & 0x00FF;
			pc += 2;
			break;
			
		case 0x7000:
			// 0x7XNN : Set Vx = Vx + NN;
			*Vx += opcode & 0x00FF;
			pc += 2;
			break;
		
		// cases 0x8...
		case 0x8000:		
			// cases 0x8XY?
			switch (opcode & 0x000F) 
			{
				// 0x8XY0 : Sets Vx = Vy.
				case 0x0000:
					*Vx = *Vy;
					break;
					
				// 0x8XY1 : Sets Vx = Vx or Vy
				case 0x0001:
					*Vx = *Vx | *Vy;
					break;
				
				// 0x8XY2 : Sets Vx = Vx and Vy
				case 0x0002:
					*Vx = *Vx & *Vy;
					break;
				
				// 0x8XY2 : Sets Vx = Vx XOR Vy
				case 0x0003:
					*Vx = *Vx ^ *Vy;
					break;
				
				// 0x8XY4 : Vx = Vx + Vy. Vf is set to 1 when there's a carry, and to 0 when there isn't.
				case 0x0004:
					// Vf = 1 if Vx > (255 - Vy)
					*Vf = *Vx > (0xFF - *Vy)
					*Vx += *Vy;
					break;
					
				// 0x8XY5 : Set Vx = Vx - Vy, set VF = NOT borrow.
				case 0x0005:
					// Vf = 1 if Vx > Vy
					*Vf = *Vx > *Vy;
					*Vx -= *Vy;
					break;
				
				// 0x8XY6 : Set Vx = Vx SHR 1.
				case 0x0006:
					// Vf = 1 if Vx is odd
					*Vf = *Vx & 0x1;
					*Vx = *Vx >> 1;
					break;
				
				// 0x8XY7 : Set Vx = Vy - Vx, set Vf = NOT borrow.
				case 0x0007:
					// Vf = 1 if Vy > Vx
					*Vf = *Vy > *Vx;
					*Vx = *Vy - *Vx;
					break;
				
				// 0x8XYE : Set Vx = Vx SHL 1.
				case 0x000E:
					// Vf = most significant byte of Vx
					*Vf = *Vx >> 7;
					*Vx = *Vx << 1;
					break;
				
				default:
					unknownOpcode(opcode);
			}
			
			pc += 2;
			break;
		
		case 0x9000:
			// 0x9XY0 : Skip next instruction if Vx != Vy.
			pc += (*Vx != *Vy) ? 4 : 2;
			break;
		
		// 0xANNN : Sets I to the address NNN.
		case 0xA000:
			regI = opcode & 0x0fff;
			pc += 2;
			break;
		
		// 0xBNNN : Jump to location NNN + V0.
		case 0xB000:
			regPc = regV[0] + (opcode & 0x0FFF);
			break;
		
		// 0xCXNN : Set Vx = random byte AND NN.
		case 0xC000:
			*Vx = (rand() % 256) & (opcode & 0x00FF);
			pc += 2;
			break;
		
		// 0xDXYN : Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
		case 0xD000:
			// TODO
			
		case 0xE000:
			switch (opcode & 0x00FF) 
			{
				// 0xEX9E : Skip next instruction if key with the value of Vx is pressed.
				case 0x009E:
					// TODO
					
				// 0xEXA1 : Skip next instruction if key with the value of Vx is not pressed.
				case 0x00A1:
					// TODO
				
				default:
					unknownOpcode(opcode);
			}
			
			break;
		
		case 0xF000:
			switch (opcode & 0x00FF) 
			{
				// 0xFX07 : Set Vx = delay timer value.
				case 0x0007:
					*Vx = delayTimer;
					break;
				
				// 0xFX0A : Wait for a key press, store the value of the key in Vx.
				case 0x000A:
					// TODO
				
				// 0xFX15 : Set delay timer = Vx.
				case 0x0015:
					delayTimer = *Vx;
					break;
					
				// 0xFX18 : Set sound timer = Vx.
				case 0x0018:
					soundTimer = *Vx;
					break;
				
				// 0xFX1E : Set I = I + Vx.
				case 0x001E:
					regI += *Vx
					break;
				
				// 0xFX29 : Set I = location of sprite for digit Vx.
				case 0x0029:
					// TODO
				
				// 0xFX33 : Store BCD representation of Vx in memory locations I, I+1, and I+2.
				case 0x0033:
					memory[regI] = *Vx / 100;
					memory[regI + 1] = (*Vx / 10) % 10;
					memory[regI + 2] = (*Vx % 100) % 10;  
					break;
				
				// 0xFX55 : Store registers V0 through Vx in memory starting at location I.
				case 0x0055:
					// TODO
					
				// 0xFX65 : Read registers V0 through Vx from memory starting at location I.
				case 0x0065:
					// TODO
				
				default:
					unknownOpcode(opcode);
			}
		
			pc += 2;
			break;
		
		default:
			unknownOpcode(opcode);
	} // fin du switch
}// fin du decode

void Chip8::unknownOpcode(u16int opcode) {
	printf("Unkown opcode : %X", opcode);
}