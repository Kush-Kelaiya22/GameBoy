#ifndef __INC__
#define __INC__

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * Declaring data types names to avoid large names.
	 */

	typedef unsigned char Byte;
	typedef unsigned short Word;
	typedef unsigned int ui32;
	typedef signed char SByte;

/**
 * Making DLL format available for easy updating and future modification.
 */
#ifdef DLL
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#ifdef __cplusplus
}
#endif

#endif // __INC__