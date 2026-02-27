#include <inc/inc.hpp>

#ifndef __BUS__
#define __BUS__

namespace gameboy
{
	class RAM;
	class CART;
	class PPU;
	class CPU;
	class BUS
	{
	private:
		RAM* ram;
		CART* cart;
		PPU* ppu;
		CPU* cpu;
	public:
		void set_ram(RAM* r);
		void set_cart(CART* c);
		void set_ppu(PPU* p);
		void set_cpu(CPU* c);
		BUS();
		Byte read(Word Addr);
		void write(Word Addr, Byte data);
	};
}

#endif // __BUS__