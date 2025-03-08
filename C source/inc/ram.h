#include <inc/inc.h>

#ifndef __RAM__
#define __RAM__

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct gbRam
	{
		_Atomic(Byte) *memory;
		void (*init)(void);
		void (*destroy)(void);
		Byte (*read)(Word);
		void (*write)(Word, Byte);
	} gbRam;

#ifdef __cplusplus
}
#endif

#endif // __RAM__