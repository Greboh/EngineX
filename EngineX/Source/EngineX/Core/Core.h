#pragma once

#ifdef ENX_PLATFORM_WINDOWS

#else
        #error EngineX only supports Windows!
#endif

#ifdef ENX_ENABLE_ASSERTS
        #define ENX_ASSERT(x, ...) {if(!(x)) {ENX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
        #define ENX_ENGINE_ASSERT(x, ...) {if(!(x)) {ENX_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
        #define ENX_ASSERT(x, ...) 
        #define ENX_ENGINE_ASSERT(x, ...) 
#endif