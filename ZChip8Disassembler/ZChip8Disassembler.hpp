#ifndef CHIP8DISASSEMBLER_HPP
#define CHIP8DISASSEMBLER_HPP

namespace Disassembler {
	
	typedef unsigned char u8int;
	typedef unsigned short u16int;
	
	void (*Chip8Instructions[16])() = 
	{
		FirstNibbleIs_0,
		FirstNibbleIs_1,
		FirstNibbleIs_2,
		FirstNibbleIs_3,
		FirstNibbleIs_4,
		FirstNibbleIs_5,
		FirstNibbleIs_6,
		FirstNibbleIs_7,
		FirstNibbleIs_8,
		FirstNibbleIs_9,
		FirstNibbleIs_A,
		FirstNibbleIs_B,
		FirstNibbleIs_C,
		FirstNibbleIs_D,
		FirstNibbleIs_E,
		FirstNibbleIs_F
	};
	
	u16int opcode;
	u8int Vx; // 0X00 in opcode
	u8int Vy; // 00Y0 in opcode
	u8int kk; // last 2 nibbles of opcode
	u16int nnn; // last 3 nibbles of opcode

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

}

#endif