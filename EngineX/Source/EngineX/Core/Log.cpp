#include "enxpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace EngineX
{
    std::shared_ptr<spdlog::logger> Log::_engineLogger;
    std::shared_ptr<spdlog::logger> Log::_clientLogger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        _engineLogger = spdlog::stdout_color_mt("EngineX");
        _engineLogger->set_level(spdlog::level::trace);

        _clientLogger = spdlog::stdout_color_mt("Application");
        _clientLogger->set_level(spdlog::level::trace);
    }
}
