#pragma once

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace EngineX
{
    class Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
        static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static Ref<spdlog::logger> s_EngineLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };
}

// Engine log macros
#define ENX_ENGINE_TRACE(...)          ::EngineX::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define ENX_ENGINE_INFO(...)           ::EngineX::Log::GetEngineLogger()->info(__VA_ARGS__)
#define ENX_ENGINE_WARN(...)           ::EngineX::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define ENX_ENGINE_ERROR(...)          ::EngineX::Log::GetEngineLogger()->error(__VA_ARGS__)
#define ENX_ENGINE_CRITICAL(...)       ::EngineX::Log::GetEngineLogger()->critical(__VA_ARGS__)

// Client log macros
#define ENX_TRACE(...)                 ::EngineX::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ENX_INFO(...)                  ::EngineX::Log::GetClientLogger()->info(__VA_ARGS__)
#define ENX_WARN(...)                  ::EngineX::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ENX_ERROR(...)                 ::EngineX::Log::GetClientLogger()->error(__VA_ARGS__)
#define ENX_CRITICAL(...)              ::EngineX::Log::GetClientLogger()->critical(__VA_ARGS__)
