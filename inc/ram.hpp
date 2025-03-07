#include <inc/inc.hpp>

#ifndef __RAM__
#define __RAM__

namespace gameboy
{
	class RAM
	{
		private:
		// The memory of the system
		static Byte* memory;
		public:
		RAM();
		~RAM();
		Byte operator[](Word Addr) const;
		Byte& operator[](Word Addr);
	};
}

#endif // __RAM__