#ifndef ZCHIP8DISASSEMBLER_HPP
#define ZCHIP8DISASSEMBLER_HPP

typedef unsigned char u8int;
typedef unsigned short u16int;

namespace Disassembler {

	void unknownOpcode();
	void FirstNibbleIs_0();
	void FirstNibbleIs_1();
	void FirstNibbleIs_2();
	void FirstNibbleIs_3();
	void FirstNibbleIs_4();
	void FirstNibbleIs_5();
	void FirstNibbleIs_6();
	void FirstNibbleIs_7();
	void FirstNibbleIs_8();
	void FirstNibbleIs_9();
	void FirstNibbleIs_A();
	void FirstNibbleIs_B();
	void FirstNibbleIs_C();
	void FirstNibbleIs_D();
	void FirstNibbleIs_E();
	void FirstNibbleIs_F();
	
	extern u16int opcode;
	extern u8int Vx; // 0X00 in opcode
	extern u8int Vy; // 00Y0 in opcode
	extern u8int kk; // last 2 nibbles of opcode
	extern u16int nnn; // last 3 nibbles of opcode
	extern void (*Chip8Instructions[16])();
	
	u8int* readFileBytes(char* name);
	
}

#endif