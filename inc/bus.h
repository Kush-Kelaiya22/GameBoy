#include <inc/inc.h>

#ifndef __BUS__
#define __BUS__

#ifdef __cplusplus
extern "C"
{
#endif

	// The system bus to manage the data handling
	typedef struct gbBus
	{
		// Read data from the bus
		Byte API (*read)(Word);
		// Write data to the bus
		void API (*write)(Word, Byte);
	} gbBus;

#ifdef __cplusplus
}
#endif

#endif // __BUS__