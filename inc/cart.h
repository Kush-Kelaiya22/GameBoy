#include <inc/inc.h>
#include <stdio.h>

#ifndef __CART__
#define __CART__

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct gbCart
	{
		// copying the entire rom in memory, as the gameboy roms are small in size.
		_Atomic(Byte) *ROM;
		// Cartridge RAM for those games that support it.
		_Atomic(Byte) *RAM;
		// ROM bank selector for those that support ROM banking
		_Atomic(Byte) ROM_BANK_NUMBER, ROM_BANK_SELECT[2];
		// RAM bank selector for those that support RAM banking
		_Atomic(Byte) RAM_BANK_NUMBER, RAM_BANK_SELECT[2];
		// Detect which MBC1 mode i used
		_Atomic(bool) MBC1_Mode;
		// RAM enable for those that have RAM
		_Atomic(bool) RAM_BANK_ENABLE[2];
		// Read data from cart
		Byte (*read)(Word);
		// Write data to cart
		void (*write)(Word, Byte);
		// cartrige setup
		void (*init)(void *);
	} gbCart;

#ifdef __cplusplus
}
#endif

#endif // __CART__