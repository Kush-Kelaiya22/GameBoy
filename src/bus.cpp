#include <inc/gameboy.hpp>

namespace gameboy
{
	BUS::BUS()
	{
	}

	Byte BUS::read(Word Addr)
	{
		Byte data;
		if (Addr >= 0x0000 and Addr <= 0x7FFF)
		{
			data = this->cart->read(Addr);
		}
		else if (Addr >= 0xC000 and Addr <= 0xFDFF)
		{
			data = this->ram->read(Addr);
		}
		else if (Addr >= 0xFF80 and Addr <= 0xFFFE)
		{
			data = this->ram->read(Addr);
		}
		else
		{
			data = 0xFF;
		}
		return data;
	}

	void BUS::write(Word Addr, Byte data)
	{
		if (Addr >= 0x0000 and Addr <= 0x7FFF)
		{
			this->cart->write(Addr, data);
		}
		else if (Addr >= 0xC000 and Addr <= 0xFDFF)
		{
			this->ram->write(Addr, data);
		}
		else if (Addr >= 0xFF80 and Addr <= 0xFFFE)
		{
			this->ram->write(Addr, data);
		}
	}
}