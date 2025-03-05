#include <inc/inc.h>

#ifndef __CPU__
#define __CPU__

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct gbCpu
	{
		// The status register containing flag data
		_Atomic(Byte) flag;
		// All the registers available in the cpu except the status register
		_Atomic(Byte) reg[8];
		// Stack Pointer
		_Atomic(Word) SP;
		// Program Counter
		_Atomic(Word) PC;
		// Interrupt Enable Flag
		_Atomic(Byte) IE;
		// Number of cycles completed
		_Atomic(ui32) ticks;
		// The instruction register
		_Atomic(Byte) ins;
		//
		_Atomic(bool) IME;
		// Internal RAM that comes with the system;
		_Atomic(Byte) *RAM;
		// Initialise the cpu and the system
		void (*init)(void);
		// Execute the logic of cpu on seperate thread to avoid performance drops.
		void (*cpu)(void);
	} gbCpu;

#ifdef __cplusplus
}
#endif

#endif // __CPU__