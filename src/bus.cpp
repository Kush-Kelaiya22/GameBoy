#include <inc/gameboy.hpp>

namespace gameboy 
{
	BUS::BUS(RAM& r, CART& c) : ram(r), cart(c)
	{}

	Byte BUS::read(Word Addr)
	{}

	void BUS::write(Word Addr, Byte data)
	{}
}