typedef unsigned char u8int;
typedef unsigned short u16int;

class Chip8
{
public:
	void Chip8();
	
	void emulateCycle();
	
	
private:
	u8int memory[0x1000]; // 4096 8 bits registers
	
	u8int regV[0x10]; // 16 registers, V0 to (VF = carry flag)
	
	u16int regI; // address register
	u16int regPc; // program counter
	
	u16int stack[0x10];
	u8int sp;
	
	u8int delayTimer;
	u8int soundTimer;
	
	u8int hexKey[0x10];
	u8int gfx[64*32];

}