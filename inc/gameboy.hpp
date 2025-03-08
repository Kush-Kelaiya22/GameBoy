#include <inc/bus.hpp>
#include <inc/cart.hpp>
#include <inc/cpu.hpp>
#include <inc/ram.hpp>
#include <inc/ins.hpp>

#ifndef __GAMEBOY__
#define __GAMEBOY__

namespace gameboy
{
	static CART cart;
	static RAM ram;
	static BUS bus(ram, cart);
	static CPU cpu(bus);
}

#endif // __GAMEBOY__