#include <inc/inc.hpp>
#include <string>

#ifndef __CART__
#define __CART__

namespace gameboy
{
	class CART
	{
	private:
		struct FILE;
		static constexpr Byte ram_lut[] = {
			[0x00] = 0,
			[0x01] = 0,
			[0x02] = 8,
			[0x03] = 32,
			[0x04] = 128,
			[0x05] = 64
		};
		static constexpr Byte rom_lut[] = {
			[0x00] = 0,
			[0x01] = 1,
			[0x02] = 2,
			[0x03] = 3,
			[0x04] = 4,
			[0x05] = 5,
			[0x06] = 6,
			[0x07] = 7,
			[0x08] = 8
		};
		// static constexpr bool ram_enable_lut[] = {
		// 	[0x00] = false,
		// 	[0x01] = false,
		// 	[0x02] = true,
		// 	[0x03] = true,
		// 	[0x05] = false,
		// 	[0x06] = false,
		// 	[0x08] = true,
		// 	[0x09] = true,
		// 	[0x0B] = false,
		// 	[0x0C] = true,
		// 	[0x0D] = true,
		// 	[0x0F] = false,
		// 	[0x10] = true,
		// 	[0x11] = false,
		// 	[0x12] = true,
		// 	[0x13] = true,
		// 	[0x19] = false,
		// 	[0x1A] = true,
		// 	[0x1B] = true,
		// 	[0x1C] = false,
		// 	[0x1D] = true,
		// 	[0x1E] = true,
		// 	[0x20] = false,
		// 	[0x22] = true,
		// 	[0xFE] = false,
		// 	[0xFF] = true
		// };
		std::string ROM_NAME;
		Byte* ROM;
		Byte* RAM;
		Byte ROM_BANK_NUMBER, ROM_BANK_SELECT[2];
		Byte RAM_BANK_NUMBER, RAM_BANK_SELECT[2];
		bool MBC1_Mode;
		bool RAM_BANK_ENABLE[2];
	public:
		CART(std::string filename);
		~CART();
		Byte read(Word Addr);
		void write(Word Addr, Byte data);
	};
}

#endif // __CART__