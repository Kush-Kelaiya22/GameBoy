#include <inc/inc.hpp>

#ifndef __CART__
#define __CART__

namespace gameboy
{
	class cart
	{
		private:
		// copying the entire rom in memory, as the gameboy roms are small in size.
		static Byte * ROM;
		// Cartridge RAM for those games that support it.
		static Byte * RAM;
		// ROM bank selector for those that support ROM banking
		static Byte ROM_BANK_NUMBER, ROM_BANK_SELECT[2];
		// RAM bank selector for those that support RAM banking
		static Byte RAM_BANK_NUMBER, RAM_BANK_SELECT[2];
		// Detect which MBC1 mode i used
		static bool MBC1_Mode;
		// RAM enable for those that have RAM
		static bool RAM_BANK_ENABLE[2];
		public:
		cart();
		~cart();
		Byte operator[](Word addr) const;
		Byte& operator[](Word addr);
	};
}

#endif // __CART__