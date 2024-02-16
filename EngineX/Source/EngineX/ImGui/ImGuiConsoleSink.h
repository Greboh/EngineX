#pragma once
#include "EngineX/Core/Core.h"
#include "spdlog/sinks/base_sink.h"

namespace EngineX
{
    class ImGuiConsoleSink : public spdlog::sinks::sink
    {
    public:
        explicit ImGuiConsoleSink(const Ref<ImGuiConsole>& console) : m_Console(console)
        {
            const int success = m_Console != nullptr;
            ENX_ENGINE_ASSERT(success, "ImGuiConsole is null!")

            m_Initialized = true;
        }

        ~ImGuiConsoleSink() override = default;

        void log(const spdlog::details::log_msg& msg) override
        {
            if (!m_Initialized)
            {
                return;
            }

            // Convert time point to string
            auto timePoint = std::chrono::system_clock::to_time_t(msg.time);
            std::tm tm = *std::localtime(&timePoint);
            std::ostringstream timeStream;
            timeStream << std::put_time(&tm, "%H:%M:%S");

            // Build the log string
            const std::string logString = fmt::format
            (
                "[{}] ({}) {}: {}\n",
                timeStream.str(),
                to_string_view(msg.level),
                msg.logger_name,
                msg.payload
            );
            
            m_Console->AddLog(logString.c_str());
        }

        void flush() override {}

        void set_pattern(const std::string& pattern) override {}

        void set_formatter(Scope<spdlog::formatter> sink_formatter) override{}

    private:
        Ref<ImGuiConsole> m_Console;
        bool m_Initialized = false;
    };
}
