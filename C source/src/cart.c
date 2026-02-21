#include <inc/cart.h>
#include <malloc.h>

static void init(void)
{
	extern gbCart gameboyCartridge;
	// FILE*fp=fopen("../Tetris.gb", "rb");
	gameboyCartridge.ROM = (_Atomic(Byte)*) malloc(0x8000);
	// fclose(fp);
}

static void destroy(void)
{
	extern gbCart gameboyCartridge;
	free((void*)gameboyCartridge.ROM);
}

static Byte read(Word Addr)
{
	extern gbCart gameboyCartridge;
	Byte Data;
	if (Addr >= 0x0000 && Addr <= 0x7FFF)
	{
		Data = gameboyCartridge.ROM[Addr];
	}
	else
	{
		Data = 0xFF;
	}
	return Data;
}

static void write(Word Addr, Byte Data)
{
}

gbCart gameboyCartridge = {
	.init = init,
	.destroy = destroy,
	.read = read,
	.write = write
};
