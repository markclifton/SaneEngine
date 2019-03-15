// Contents of DLLDefines.h
#ifndef _MyLibrary_DLLDEFINES_H_
#define _MyLibrary_DLLDEFINES_H_

#if defined (_WIN32)
#if defined(MyLibrary_EXPORTS)
#define  MYLIB_EXPORT __declspec(dllexport)
#else
#define  MYLIB_EXPORT __declspec(dllimport)
#endif
#else
#define MYLIB_EXPORT
#endif

#endif
