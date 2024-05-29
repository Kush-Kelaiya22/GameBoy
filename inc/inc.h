#ifndef __INC__
#define __INC__

#ifdef __cplusplus
extern "C"
{
#endif

	typedef unsigned char Byte;
	typedef unsigned short Word;
	typedef unsigned int ui32;
	typedef signed char SByte;

#ifdef DLL
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#ifdef __cplusplus
}
#endif

#endif // __INC__