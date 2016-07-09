#include <cstdio>
#include <fstream>

#include "ZChip8Disassembler.hpp"

namespace Disassembler {
	
void FirstNibbleIs_0()
{
	switch (opcode & 0x00FF)
	{
		// 00E0 : Clear the display.
		case 0xE0:
			printf("%-16s : %s", "CLS", "Clear the display");
			break;
		
		// 00EE : Return from a subroutine.
		case 0xEE:
			printf("%-16s : %s", "RET", "Return from a subroutine");
			break;
		
		default:
			unknownOpcode();
	}
}

void FirstNibbleIs_1()
{
	// 1nnn : Jump to location nnn.
	printf("JUMP &0x%03X%5s : Jump to location &%03X", nnn, "", nnn);
}

void FirstNibbleIs_2()
{
	// 2nnn : Call subroutine at nnn.
	printf("CALL &0x%03X%5s : Call subroutine at &%03X", nnn, "", nnn);
}

void FirstNibbleIs_3()
{
	// 3xkk : Skip next instruction if Vx = kk.
	printf("SKPE V%01X, $0x%02X%2s : Skip next instruction if V%d = %d", Vx, kk, "", Vx, kk);
}

void FirstNibbleIs_4()
{
	// 4xkk : Skip next instruction if Vx != kk.
	printf("SKIPNE V%01X, $0x%02X : Skip next instruction if V%d != %d", Vx, kk, Vx, kk);
}

void FirstNibbleIs_5()
{
	// 5xy0 : Skip next instruction if Vx = Vy.
	printf("SKIPE V%01X, V%01X%4s : Skip next instruction if V%d = V%d", Vx, Vy, "", Vx, Vy);
}

void FirstNibbleIs_6()
{
	// 6xkk : Set Vx = kk.
	printf("LOAD V%01X, $0x%02X%2s : Set V%d = %d", Vx, kk, "", Vx, kk);
}

void FirstNibbleIs_7()
{
	// 7xkk : Set Vx = Vx + kk.
	printf("ADD V%01X, $0x%02X%3s : Set V%d = V%d + %d", Vx, kk, "", Vx, Vx, kk);
}

void FirstNibbleIs_8()
{
	switch (opcode & 0xF) 
	{
		// 8xy0 : Set Vx = Vy.
		case 0x0:
			printf("LOAD V%01X, V%01X%5s : Set V%d = V%d", Vx, Vy, "", Vx, Vy);
			break;
		
		// 8xy1 : Set Vx = Vx OR Vy.
		case 0x1:
			printf("OR V%01X, V%01X%7s : Set V%d = V%d OR V%d", Vx, Vy, "", Vx, Vx, Vy);
			break;
		
		// 8xy2 : Set Vx = Vx AND Vy.
		case 0x2:
			printf("AND V%01X, V%01X%6s : Set V%d = V%d AND V%d", Vx, Vy, "", Vx, Vx, Vy);
			break;
		
		// 8xy3 : Set Vx = Vx XOR Vy.
		case 0x3:			
			printf("XOR V%01X, V%01X%6s : Set V%d = V%d XOR V%d", Vx, Vy, "", Vx, Vx, Vy);
			break;
			
		// 8xy4 : Set Vx = Vx + Vy, set VF = carry.
		case 0x4:
			printf("ADD V%01X, V%01X%6s : Set V%d = V%d + V%d", Vx, Vy, "", Vx, Vx, Vy);
			break;
		
		// 8xy5 : Set Vx = Vx - Vy, set VF = NOT borrow.
		case 0x5:
			printf("SUB V%01X, V%01X%6s : Set V%d = V%d - V%d", Vx, Vy, "", Vx, Vx, Vy);
			break;
		
		// 8xy6 : Set Vx = Vx SHR 1.
		case 0x6:
			printf("SHIFTR V%01X,%6s : Set V%d = V%d >> 1", Vx, "", Vx, Vx);
			break;
		
		// 8xy7 : Set Vx = Vy - Vx, set VF = NOT borrow.
		case 0x7:
			printf("SUBN V%01X, V%01X%5s : Set V%d = V%d - V%d", Vx, Vy, "", Vx, Vy, Vx);
			break;
		
		// 8xyE : Set Vx = Vx SHL 1.
		case 0xE:
			printf("SHIFTL V%01X,%6s : Set V%d = V%d << 1", Vx, "", Vx, Vx);
			break;
			
		default:
			unknownOpcode();
		
	}
}
void FirstNibbleIs_9()
{
	// 9xy0 : Skip next instruction if Vx != Vy.
	printf("SKIPNE V%01X, V%01X%3s : Skip next instruction if V%d != V%d", Vx, Vy, "", Vx, Vy);
}

void FirstNibbleIs_A()
{
	// Annn : Set I = nnn.
	printf("LOAD I, &0x%04X%1s : Set I = &0x%04X", nnn, "", nnn);
}

void FirstNibbleIs_B()
{
	// Bnnn : Jump to location nnn + V0.
	printf("JUMP V0, &0x%03X%1s : Jump to location V0 + &0x%03X", nnn, "", nnn);
}

void FirstNibbleIs_C()
{
	// Cxkk : Set Vx = random byte AND kk.
	printf("RAND V%01X, $0x%02X%2s : Set V%d = random byte AND $0x%02X", Vx, kk, "", Vx, kk);
}

void FirstNibbleIs_D()
{
	// Dxyn : Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	printf("DRW V%01X, V%01X, $0x%01X : Display $0x%01X-byte sprite starting at memory loc. I at (V%d, V%d)", Vx,Vy, kk & 0xF, kk & 0xF, Vx, Vy);
}

void FirstNibbleIs_E()
{
	switch (kk) 
	{
		// Ex9E : Skip next instruction if key with the value of Vx is pressed.
		case 0x9E:
			printf("SKIP V%01X%9s : Skip next instruction if key with value V%d is pressed.", Vx, "", Vx);
			break;
		
		// ExA1 : Skip next instruction if key with the value of Vx is not pressed.
		case 0xA1:
			printf("SKINP V%01X%8s : Skip next instruction if key with value V%d is not pressed.", Vx, "", Vx);
			break;
		
		default:
			unknownOpcode();
	}
}

void FirstNibbleIs_F()
{
	switch (kk)
	{
		// Fx07 : Set Vx = delay timer value.
		case 0x07:
			printf("LOAD V%01X, DT%5s : Set V%d = DelayTimer value", Vx, "", Vx);
			break;
		
		// Fx0A : Wait for a key press, store the value of the key in Vx.
		case 0x0A:
			printf("LOAD V%01X%9s : Wait for a key press, store the value of the key in V%d", Vx, "", Vx);
			break;
		
		// Fx15 : Set delay timer = Vx.
		case 0x15:
			printf("LOAD DT, V%01X%5s : Set DelayTimer = V%d", Vx, "", Vx);
			break;
		
		// Fx18 : Set sound timer = Vx.
		case 0x18:
			printf("LOAD ST, V%01X%5s : Set SoundTimer = V%d", Vx, "", Vx);		
			break;
			
		// Fx1E : Set I = I + Vx.
		case 0x1E:
			printf("ADD I, V%01X%7s : Set I = I + V%d", Vx, "", Vx);
			break;
		
		// Fx29 : Set I = location of sprite for digit Vx.
		case 0x29:
			printf("LOAD F, V%01X%6s : Set I = location of sprite for digit V%d", Vx, "", Vx);
			break;
		
		// Fx33 : Store BCD representation of Vx in memory locations I, I+1, and I+2.
		case 0x33:
			printf("LOAD B, V%01X%6s : Store BCD representation of V%d in memory[I:I+3]", Vx, "", Vx);
			break;
			
		// Fx55 : Store registers V0 through Vx in memory starting at location I.
		case 0x55:
			printf("LOAD [I], V%01X%4s : Store registers V0 to V%d in memory starting at I", Vx, "", Vx);
			break;
		
		// Fx65 : Read registers V0 through Vx from memory starting at location I.
		case 0x65:
			printf("LOAD V%01X, [I]%4s : Read registers V0 to V%d from memory starting at I", Vx, "", Vx);
			break;
		
		default:
			unknownOpcode();
	}
}

void unknownOpcode()
{
	printf("%-16s : %s", "UNKNOWN OPCODE", "NOT IN THE OPCODE LIST");
}

u8int* readFileBytes(std::ifstream file)  
{  
 
}  

} // End of neamespace
