﻿#include "enxpch.h"

#include "EngineX/ImGui/ImGuiConsoleSink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace EngineX
{
    Ref<spdlog::logger> Log::s_EngineLogger;
    Ref<spdlog::logger> Log::s_ClientLogger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_EngineLogger = spdlog::stdout_color_mt("EngineX");
        s_EngineLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("Application");
        s_ClientLogger->set_level(spdlog::level::trace);
    }

    void Log::AttachImGuiConsoleSink(const std::shared_ptr<ImGuiConsole>& console)
    {
        // Then, create an instance of ImGuiConsoleSink using the shared pointer to myImGuiConsole
        auto const sinkInstance = std::make_shared<ImGuiConsoleSink>(console);
        spdlog::get("Application")->sinks().push_back(sinkInstance);
    }
}
