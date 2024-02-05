#pragma once


#ifdef ENX_PLATFORM_WINDOWS
#ifdef ENX_BUILD_DLL
#define ENGINEX_API __declspec(dllexport)
#else
        #define ENGINEX_API __declspec(dllimport)
#endif
#else
#error EngineX only supports Windows!
#endif
