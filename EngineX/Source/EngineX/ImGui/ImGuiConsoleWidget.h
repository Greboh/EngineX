#pragma once
#include <spdlog/common.h>

#include "imgui.h"

namespace EngineX
{
    class ImGuiConsole
    {
    public:
        ImGuiTextBuffer Buf;
        ImGuiTextFilter Filter;
        ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        bool AutoScroll; // Keep scrolling if already at the bottom.

        ImGuiConsole()
        {
            AutoScroll = true;
            Clear();
        }

        // Function to extract the log level from the message string

        void Clear();

        void AddLog(const char* fmt, ...);

        void Draw(const char* title, bool* p_open = NULL);

        static spdlog::level::level_enum GetLogLevel(const std::string& message)
        {
            // Find the opening and closing parentheses
            size_t open_pos = message.find("(");
            size_t close_pos = message.find(")");

            // If both parentheses are found
            if (open_pos != std::string::npos && close_pos != std::string::npos)
            {
                // Extract the substring between parentheses
                std::string level_str = message.substr(open_pos + 1, close_pos - open_pos - 1);

                // Check if the substring matches known log level strings
                if (level_str == "error")
                    return spdlog::level::err;
                if (level_str == "warning")
                    return spdlog::level::warn;
                if (level_str == "info")
                    return spdlog::level::info;
            }

            // Default to info level if the level couldn't be extracted
            return spdlog::level::trace;
        }
    };
    
}
