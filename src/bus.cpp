#include <inc/gameboy.hpp>

namespace gameboy
{
	BUS::BUS()
	{
	}

	void BUS::set_ram(RAM* r)
	{
		this->ram = r;
	}

	void BUS::set_cart(CART* c)
	{
		this->cart = c;
	}

	void BUS::set_ppu(PPU* p)
	{
		this->ppu = p;
	}

	void BUS::set_cpu(CPU* c)
	{
		this->cpu = c;
	}

	BUS::~BUS()
	{
		this->ram = nullptr;
		this->cart = nullptr;
		this->ppu = nullptr;
		this->cpu = nullptr;
	}

	Byte BUS::read(Word Addr)
	{
		Byte data;
		if (Addr >= 0x0000 and Addr <= 0x7FFF)
		{
			data = this->cart->read(Addr);
		}
		else if (Addr >= 0x8000 and Addr <= 0x9FFF)
		{
			data = this->ppu->read(Addr);
		}
		else if (Addr >= 0xA000 and Addr <= 0xBFFF)
		{
			data = this->cart->read(Addr);
		}
		else if (Addr >= 0xC000 and Addr <= 0xFDFF)
		{
			data = this->ram->read(Addr & 0xDFFF);
		}
		else if (Addr >= 0xFE00 and Addr <= 0xFE9F)
		{
			data = this->ppu->read(Addr);
		}
		else if (Addr >= 0xFF80 and Addr <= 0xFFFE)
		{
			data = this->ram->read(Addr);
		}
		else if (Addr == 0xFF00)
		{
			data = this->cpu->IME ? 0x01 : 0x00;
		}
		else
		{
			data = 0x00;
		}
		return data;
	}

	void BUS::write(Word Addr, Byte data)
	{
		if (Addr >= 0x0000 and Addr <= 0x7FFF)
		{
			this->cart->write(Addr, data);
		}
		else if (Addr >= 0x8000 and Addr <= 0x9FFF)
		{
			this->ppu->write(Addr, data);
		}
		else if (Addr >= 0xFE00 and Addr <= 0xFE9F)
		{
			this->ppu->write(Addr, data);
		}
		else if (Addr >= 0xC000 and Addr <= 0xFDFF)
		{
			this->ram->write(Addr, data);
		}
		else if (Addr >= 0xFF80 and Addr <= 0xFFFE)
		{
			this->ram->write(Addr, data);
		}
		else if (Addr == 0xFF00)
		{
			this->cpu->IME = data & 0x01;
		}
		else
		{
			// Do nothing
		}
	}
}