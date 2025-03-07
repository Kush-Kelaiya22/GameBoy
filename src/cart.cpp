#include <inc/cart.hpp>

namespace gameboy
{
	cart::cart()
	{
	}

	cart::~cart()
	{
		delete[] ROM;
		delete[] RAM;
	}

	Byte cart::operator[](Word Addr) const
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

	Byte& cart::operator[](Word Addr)
	{
		if (Addr >= 0xA000 and Addr <= 0xBFFF)
		{
			return RAM[Addr & 0x1FFF];
		}
	}
}