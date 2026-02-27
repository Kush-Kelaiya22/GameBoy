#include <inc/gameboy.hpp>

namespace gameboy
{
	RAM::RAM() {}

	RAM::~RAM() {}

	Byte RAM::read(Word Addr)
	{
		if (Addr >= 0xFF80 && Addr <= 0xFFFE)
			return this->hram[Addr - 0xFF80];
		else
			return this->memory[Addr & 0x1FFF];
	}

	void RAM::write(Word Addr, Byte data)
	{
		if (Addr >= 0xFF80 && Addr <= 0xFFFE)
			this->hram[Addr - 0xFF80] = data;
		else
			this->memory[Addr & 0x1FFF] = data;
	}
}