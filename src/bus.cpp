#include <inc/gameboy.hpp>

namespace gameboy
{
	BUS::BUS(RAM& r, CART& c) : ram(r), cart(c)
	{
	}

	Byte BUS::read(Word Addr)
	{
		Byte data;
		if (Addr >= 0xC000 and Addr <= 0xFDFF)
		{
			data = this->ram[Addr & 0x1FFF];
		}
		if (Addr >= 0x0000 and Addr <= 0x7FFF)
		{
			data = this->cart[Addr & 0x7FFF];
		}
		return data;
	}

	void BUS::write(Word Addr, Byte data)
	{
		if (Addr >= 0xC000 and Addr <= 0xFDFF)
		{
			this->ram[Addr & 0x1FFF] = data;
		}
	}
}