#include <inc/inc.hpp>

#ifndef __CART__
#define __CART__

namespace gameboy
{
	class CART
	{
		private:
		Byte * ROM;
		Byte * RAM;
		Byte ROM_BANK_NUMBER, ROM_BANK_SELECT[2];
		Byte RAM_BANK_NUMBER, RAM_BANK_SELECT[2];
		bool MBC1_Mode;
		bool RAM_BANK_ENABLE[2];
		public:
		CART();
		~CART();
		Byte operator[](Word addr) const;
		Byte& operator[](Word addr);
	};
}

#endif // __CART__