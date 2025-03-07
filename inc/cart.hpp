#include <inc/inc.hpp>

#ifndef __CART__
#define __CART__

namespace gameboy
{
	class CART
	{
		private:
		static Byte * ROM;
		static Byte * RAM;
		static Byte ROM_BANK_NUMBER, ROM_BANK_SELECT[2];
		static Byte RAM_BANK_NUMBER, RAM_BANK_SELECT[2];
		static bool MBC1_Mode;
		static bool RAM_BANK_ENABLE[2];
		public:
		CART();
		~CART();
		Byte operator[](Word addr) const;
		Byte& operator[](Word addr);
	};
}

#endif // __CART__