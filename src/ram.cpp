#include <inc/ram.hpp>

namespace gameboy
{
	RAM::RAM()
	{
		memory = new Byte[0x2000];
	}

	RAM::~RAM()
	{
		delete[] memory;
	}

	Byte RAM::operator[](Word Addr) const
	{
		return memory[Addr];
	}

	Byte& RAM::operator[](Word Addr)
	{
		return memory[Addr];
	}
}