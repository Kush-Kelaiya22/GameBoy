#include <inc/gameboy.hpp>
#include <fstream>
#include <iostream>

namespace gameboy
{
	CART::CART()
	{
		std::ifstream file("rom.gb", std::ios::binary);
		if (!file)
		{
			std::cerr << "Error: Could not open ROM file." << '\n';
			exit(1);
		}
		this->RAM = new Byte[0x2000 * (1 << this->ram_lut[0x00])];
	}

	CART::~CART()
	{
		delete[] this->ROM;
		delete[] this->RAM;
	}

	Byte CART::operator[](Word Addr) const
	{
		if (Addr < 0x8000)
		{
			return this->ROM[Addr];
		}
		else if (Addr >= 0xA000 && Addr <= 0xBFFF)
		{
			return this->RAM[Addr & 0x1FFF];
		}
		else
		{
			return 0;
		}
	}

	Byte& CART::operator[](Word Addr)
	{
		return this->RAM[Addr & 0x1FFF];
	}
}