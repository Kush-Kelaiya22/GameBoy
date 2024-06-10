#include <inc/bus.h>

gbBus gameboyBUS;

Byte read(Word Addr)
{
}

void write(Word Addr, Byte Data)
{
}

gbBus gameboyBUS = {
	.write = write,
	.read = read};