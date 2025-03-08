#include <inc/ram.h>
#include <malloc.h>

void ram_init(void)
{
	extern gbRam gameboyRAM;
	gameboyRAM.memory = (_Atomic(Byte)*)malloc(0x2000);
}

void ram_destroy(void)
{
	extern gbRam gameboyRAM;
	free(gameboyRAM.memory);
	gameboyRAM.memory = nullptr;
}

Byte ram_read(Word Addr)
{
	extern gbRam gameboyRAM;
	return gameboyRAM.memory[Addr];
}

void ram_write(Word Addr, Byte Val)
{
	extern gbRam gameboyRAM;
	gameboyRAM.memory[Addr] = Val;
}

gbRam gameboyRAM = {
	.init = ram_init,
	.destroy = ram_destroy,
	.read = ram_read,
	.write = ram_write
};