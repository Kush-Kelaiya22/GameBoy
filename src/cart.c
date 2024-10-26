#include <inc/cart.h>

gbCart gameboyCartridge;

void init(void *)
{
}

Byte read(Word Addr)
{
}

gbCart gameboyCartridge = {
	.init = init,
	.read = read};