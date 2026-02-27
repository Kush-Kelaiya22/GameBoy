#include <inc/inc.hpp>

#ifndef __PPU__
#define __PPU__

namespace gameboy
{
	class PPU
	{
	private:
		Byte vram[0x2000];
		Byte oam[0xA0];
	public:
		PPU();
		~PPU();
		Byte read(Word Addr);
		void write(Word Addr, Byte data);
	};
}

#endif // __PPU__