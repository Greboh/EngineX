#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace EngineX
{
    class ENGINEX_API Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() {return _engineLogger;}
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {return _clientLogger;}

    private:
        static std::shared_ptr<spdlog::logger> _engineLogger;
        static std::shared_ptr<spdlog::logger> _clientLogger;
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