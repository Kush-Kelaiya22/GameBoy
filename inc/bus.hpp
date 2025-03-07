#include <inc/inc.hpp>

#ifndef __BUS__
#define __BUS__

namespace gameboy
{
	class BUS
	{
		public:
		Byte operator[](Word Addr) const;
		Byte& operator[](Word Addr);
	};
}

#endif // __BUS__