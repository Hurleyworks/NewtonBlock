/* Copyright (c) <2009> <Newton Game Dynamics>

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely
*/

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef __D_CONTAINERS_STDAFX__
#define __D_CONTAINERS_STDAFX__




#ifdef _DCONTAINERS_DLL
   #ifdef _DCONTAINERS_EXPORT
      #ifdef _WIN32
         #define DCONTAINERS_API __declspec (dllexport)
      #else
         #define DCONTAINERS_API __attribute__ ((visibility("default")))
      #endif
   #else
      #ifdef _WIN32
         #define DCONTAINERS_API __declspec (dllimport)
      #else
         #define DCONTAINERS_API
      #endif
   #endif
#else
   #define DCONTAINERS_API
#endif


#ifdef _MSC_VER
#endif

#ifdef _MACOSX_VER
   #ifndef _MAC_IPHONE
   #endif

#endif


#endif
