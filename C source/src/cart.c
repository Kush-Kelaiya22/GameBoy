#include <inc/cart.h>
#include <malloc.h>

void cart_init(void)
{
	extern gbCart gameboyCartridge;
	// FILE*fp=fopen("../Tetris.gb", "rb");
	gameboyCartridge.ROM = (_Atomic(Byte)*) malloc(0x8000);
	// fclose(fp);
}

void cart_destroy(void)
{
	extern gbCart gameboyCartridge;
	free((void*)gameboyCartridge.ROM);
}

Byte cart_read(Word Addr)
{
	extern gbCart gameboyCartridge;
	Byte Data;
	if(Addr >= 0x0000 && Addr <= 0x7FFF)
	{
		Data = gameboyCartridge.ROM[Addr];
	}
	else
	{
		Data = 0xFF;
	}
	return Data;
}

void cart_write(Word Addr, Byte Data)
{}

gbCart gameboyCartridge = {
	.init = cart_init,
	.destroy = cart_destroy,
	.read = cart_read,
	.write = cart_write
};
