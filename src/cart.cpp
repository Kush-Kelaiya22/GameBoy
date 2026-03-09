#include <inc/gameboy.hpp>
#include <fstream>
#include <iostream>

namespace gameboy
{
	MBC_base::MBC_base(Byte* rom, Byte* ram) : ROM(rom), RAM(ram)
	{
	}

	MBC0::MBC0(Byte* rom, Byte* ram) : MBC_base(rom, ram)
	{
	}

	Byte MBC0::read(Word Addr)
	{
		Byte data;
		if (Addr >= 0x0000 and Addr <= 0x7FFF) data = this->ROM[Addr];
		else if (Addr >= 0xA000 and Addr <= 0xBFFF) data = this->RAM[Addr & 0x1FFF];
		else throw "Invalid address read";
		return data;
	}

	void MBC0::write(Word Addr, Byte data)
	{
		if (Addr >= 0xA000 and Addr <= 0xBFFF) this->RAM[Addr & 0x1FFF] = data;
	}

	CART::CART(std::string filename)
	{
		this->ROM_NAME = filename;
		std::ifstream file(filename, std::ios::binary);
		if (!file)
		{
			std::cerr << "Error: Could not open ROM file." << this->ROM_NAME << '\n';
			exit(1);
		}
		this->ROM = new Byte[0x200];
		file.read(reinterpret_cast<char*>(this->ROM), 0x200);
		file.seekg(0, std::ios::beg);
	}

	MBC2::MBC2(Byte* rom, Byte* ram) : MBC_base(rom, ram)
	{
		this->rom_bank_ptr = (this->ROM + 0x4000);
	}

	Byte MBC2::read(Word Addr)
	{
		Byte data = 0;
		if (Addr >= 0x0000 and Addr <= 0x3FFF) data = this->ROM[Addr];
		else if (Addr >= 0x4000 and Addr <= 0x7FFF) data = this->rom_bank_ptr[Addr & 0x3FFF];
		return data;
	}

	void MBC2::write(Word Addr, Byte Data)
	{
		if (Addr & 0x0100)
		{
			bank_number = Data & 0x0F;
			if (!bank_number) bank_number = 1;
			this->rom_bank_ptr = (this->ROM + (bank_number * 0x4000));
		}
		else ram_enable = Data & 0x01;
	}

	CART::~CART()
	{
		delete[] this->ROM;
		delete[] this->RAM;
	}

	Byte CART::read(Word Addr)
	{
		return this->mbc->read(Addr);
	}

	void CART::write(Word Addr, Byte data)
	{
		this->mbc->write(Addr, data);
	}
}