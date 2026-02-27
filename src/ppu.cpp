#include <inc/ppu.hpp>

namespace gameboy
{
	PPU::PPU() {}

	PPU::~PPU() {}

	Byte PPU::read(Word Addr)
	{
		if (Addr >= 0x8000 && Addr <= 0x9FFF)
		{
			return this->vram[Addr & 0x1FFF];
		}
		else if (Addr >= 0xFE00 && Addr <= 0xFE9F)
		{
			return this->oam[Addr & 0x9F];
		}
		else
		{
			return 0xFF;
		}
	}

	void PPU::write(Word Addr, Byte data)
	{
		if (Addr >= 0x8000 && Addr <= 0x9FFF)
		{
			this->vram[Addr & 0x1FFF] = data;
		}
		else if (Addr >= 0xFE00 && Addr <= 0xFE9F)
		{
			this->oam[Addr & 0x9F] = data;
		}
	}
}