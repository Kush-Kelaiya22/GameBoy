#include <inc/gameboy.hpp>

namespace gameboy
{
	RAM::RAM()
	{
		this->memory = new Byte[0x2000];
		this->hram = new Byte[0x7F];
	}

	RAM::~RAM()
	{
		delete[] this->memory;
		delete[] this->hram;
	}

	Byte RAM::operator[](Word Addr) const
	{
		return this->memory[Addr];
	}

	Byte& RAM::operator[](Word Addr)
	{
		return this->memory[Addr];
	}
}