#include <inc/ram.h>
#include <malloc.h>

static void init(void)
{
	extern gbRam gameboyRAM;
	gameboyRAM.memory = (_Atomic(Byte)*)malloc(0x2000);
}

static void destroy(void)
{
	extern gbRam gameboyRAM;
	free(gameboyRAM.memory);
	gameboyRAM.memory = nullptr;
}

static Byte read(Word Addr)
{
	extern gbRam gameboyRAM;
	return gameboyRAM.memory[Addr];
}

static void write(Word Addr, Byte Val)
{
	extern gbRam gameboyRAM;
	gameboyRAM.memory[Addr] = Val;
}

gbRam gameboyRAM = {
	.init = init,
	.destroy = destroy,
	.read = read,
	.write = write
};