#include <inc/inc.hpp>

#ifndef __BUS__
#define __BUS__

namespace gameboy
{
	class BUS
	{
		public:
		Byte read(Word Addr);
		void write(Word Addr, Byte data);
	};
}

#endif // __BUS__