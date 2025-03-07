#include <inc/inc.hpp>

#ifndef __CPU__
#define __CPU__

namespace gameboy
{
	class CPU 
	{
		private:
		// The status register containing flag data
		static Byte flag;
		// All the registers available in the cpu except the status register
		static Byte reg[8];
		// Stack Pointer
		static Word SP;
		// Program Counter
		static Word PC;
		// Interrupt Enable Flag
		static Byte IE;
		// Number of cycles completed
		static ui32 ticks;
		// The instruction register
		static Byte ins;
		// Interrut enable register
		static bool IME;
		public:
		CPU();
		~CPU();
		void execute();
	};
}

#endif