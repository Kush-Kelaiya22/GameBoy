#include <inc/inc.hpp>

#ifndef __BUS__
#define __BUS__

namespace gameboy
{
	class RAM;
	class CART;
	class BUS
	{
	private:
		RAM& ram;
		CART& cart;
	public:
		BUS(RAM& r, CART& c);
		Byte read(Word Addr);
		void write(Word Addr, Byte data);
	};
}

#endif // __BUS__