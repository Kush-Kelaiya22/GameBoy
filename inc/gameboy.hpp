#include <inc/bus.hpp>
#include <inc/cart.hpp>
#include <inc/cpu.hpp>
#include <inc/ram.hpp>
#include <inc/ins.hpp>

#ifndef __GAMEBOY__
#define __GAMEBOY__

namespace gameboy
{
	CART cartridge;
	CPU cpu;
	BUS bus;
	RAM ram;
}

#endif // __GAMEBOY__