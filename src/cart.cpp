#include <inc/gameboy.hpp>

namespace gameboy
{
	CART::CART()
	{
	}

	CART::~CART()
	{
		delete[] ROM;
		delete[] RAM;
	}

	Byte CART::operator[](Word Addr) const
	{
		if (Addr < 0x8000)
		{
			return ROM[Addr];
		}
		else if (Addr >= 0xA000 && Addr <= 0xBFFF)
		{
			return RAM[Addr & 0x1FFF];
		}
		else
		{
			return 0;
		}
	}

	Byte& CART::operator[](Word Addr)
	{
		return RAM[Addr & 0x1FFF];
	}
}