#include <inc/bus.h>
#include <inc/cart.h>
#include <inc/ram.h>

extern gbCart gameboyCartridge;
extern gbRam gameboyRAM;

Byte read(Word Addr)
{
	Byte Data;
	if(Addr >= 0x0000 && Addr <= 0x7FFF)
		Data = gameboyCartridge.read(Addr);
	else if(Addr >= 0xC000 && Addr <= 0xFDFF)
		Data = gameboyRAM.read(Addr);
	return Data;
}

void write(Word Addr, Byte Data)
{
	if(Addr >= 0x0000 && Addr <= 0x7FFF)
		gameboyCartridge.write(Addr, Data);
	else if(Addr >= 0xC000 && Addr <= 0xFDFF)
		gameboyRAM.write(Addr & 0x2000, Data);
}

gbBus gameboyBUS = {
	.write = write,
	.read = read
};