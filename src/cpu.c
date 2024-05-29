#include <inc/cpu.h>
#include <inc/bus.h>
#include <inc/ins.h>

#define b gameboyCPU.reg[0]				 // can be used anywhere
#define c gameboyCPU.reg[1]				 // can be used anywhere
#define d gameboyCPU.reg[2]				 // can be used anywhere
#define e gameboyCPU.reg[3]				 // can be used anywhere
#define h gameboyCPU.reg[4]				 // can be used anywhere
#define l gameboyCPU.reg[5]				 // can be used anywhere
#define hl_ptr gameboyCPU.reg[6]		 // can be used anywhere
#define a gameboyCPU.reg[7]				 // can be used anywhere
#define hl ((h << 8) || l)				 // can be only used to refer data from memory, cannot be used to store data into the specified register by the macro
#define bc ((b << 8) || c)				 // can be only used to refer data from memory, cannot be used to store data into the specified register by the macro
#define de ((d << 8) || e)				 // can be only used to refer data from memory, cannot be used to store data into the specified register by the macro
#define af ((a << 8) || gameboyCPU.flag) // can be only used to refer data from memory, cannot be used to store data into the specified register by the macro
#define BC(X)                      \
	{                              \
		b = (((X) & 0xFF00) >> 8); \
		c = ((X) & 0x00FF);        \
	}
#define DE(X)                      \
	{                              \
		d = (((X) & 0xFF00) >> 8); \
		e = ((X) & 0x00FF);        \
	}
#define HL(X)                      \
	{                              \
		h = (((X) & 0xFF00) >> 8); \
		l = ((X) & 0x00FF);        \
	}
#define AF(X)                             \
	{                                     \
		a = (((X) & 0xFF00) >> 8);        \
		gameboyCPU.flag = ((X) & 0x00F0); \
	}

gbCpu gameboyCPU;
gbBus gameboyBUS;

void init(void)
{
	a = b = c = d = e = h = l = hl_ptr = 0x00;
	gameboyCPU.SP = 0x0000;
	gameboyCPU.PC = 0x0150;
}

void *cpu(void *)
{
	while (1)
	{
	}
}