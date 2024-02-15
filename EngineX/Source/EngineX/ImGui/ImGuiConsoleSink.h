#pragma once
#include "imgui.h"
#include "EngineX/Core/Core.h"
#include "spdlog/sinks/base_sink.h"

namespace EngineX
{
    // Usage:
    //  static ExampleAppLog my_log;
    //  my_log.AddLog("Hello %d world\n", 123);
    //  my_log.Draw("title");
    struct ImGuiConsole
    {
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
        spdlog::level::level_enum GetLogLevel(const std::string& message)
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

        void Clear()
        {
            Buf.clear();
            LineOffsets.clear();
            LineOffsets.push_back(0);
        }

        void AddLog(const char* fmt, ...) IM_FMTARGS(2)
        {
            int old_size = Buf.size();
            va_list args;
            va_start(args, fmt);
            Buf.appendfv(fmt, args);
            va_end(args);
            for (int new_size = Buf.size(); old_size < new_size; old_size++)
                if (Buf[old_size] == '\n')
                    LineOffsets.push_back(old_size + 1);
        }

        void Draw(const char* title, bool* p_open = NULL)
        {
            if (!ImGui::Begin(title, p_open))
            {
                ImGui::End();
                return;
            }

            // Options menu
            if (ImGui::BeginPopup("Options"))
            {
                ImGui::Checkbox("Auto-scroll", &AutoScroll);
                ImGui::EndPopup();
            }

            // Main window
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            bool clear = ImGui::Button("Clear");
            ImGui::SameLine();
            bool copy = ImGui::Button("Copy");
            ImGui::SameLine();
            Filter.Draw("Filter", -100.0f);

            ImGui::Separator();

            if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
            {
                if (clear)
                    Clear();
                if (copy)
                    ImGui::LogToClipboard();

                // Increase the font scale to make the text bigger
                const float oldFontSize = ImGui::GetFont()->Scale;
                ImGui::GetFont()->Scale *= 1.25f;
                ImGui::PushFont(ImGui::GetFont());
                
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));
                const char* buf = Buf.begin();
                const char* buf_end = Buf.end();
                if (Filter.IsActive())
                {
                    // In this example we don't use the clipper when Filter is enabled.
                    // This is because we don't have random access to the result of our filter.
                    // A real application processing logs with ten of thousands of entries may want to store the result of
                    // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                    for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size)
                                                   ? (buf + LineOffsets[line_no + 1] - 1)
                                                   : buf_end;
                        if (Filter.PassFilter(line_start, line_end))
                        {
                            // Get the log level from the message string
                            spdlog::level::level_enum level = GetLogLevel(std::string(line_start, line_end));

                            // Set color based on log level
                            ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default color (white)
                            if (level == spdlog::level::err)
                                color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); // Red color for error messages
                            else if (level == spdlog::level::warn)
                                color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f); // Yellow color for warnings
                            else if (level == spdlog::level::info)
                                color = ImVec4(0.4f, 1.0f, 0.4f, 1.0f); // Green color for info messages

                            // Apply text color
                            ImGui::PushStyleColor(ImGuiCol_Text, color);
                            
                            // Display the line
                            ImGui::TextUnformatted(line_start, line_end);
            
                            // Pop the text color
                            ImGui::PopStyleColor();
                        }
                    }
                }
                else
                {
                    // The simplest and easy way to display the entire buffer:
                    //   ImGui::TextUnformatted(buf_begin, buf_end);
                    // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                    // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                    // within the visible area.
                    // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                    // on your side is recommended. Using ImGuiListClipper requires
                    // - A) random access into your data
                    // - B) items all being the  same height,
                    // both of which we can handle since we have an array pointing to the beginning of each line of text.
                    // When using the filter (in the block of code above) we don't have random access into the data to display
                    // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                    // it possible (and would be recommended if you want to search through tens of thousands of entries).
                    ImGuiListClipper clipper;
                    clipper.Begin(LineOffsets.Size);
                    while (clipper.Step())
                    {
                        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                        {
                            // Draw a line under the text
                            ImGui::Separator();
                            
                            const char* line_start = buf + LineOffsets[line_no];
                            const char* line_end = (line_no + 1 < LineOffsets.Size)
                                                       ? (buf + LineOffsets[line_no + 1] - 1)
                                                       : buf_end;

                            // Get the log level from the message string
                            spdlog::level::level_enum level = GetLogLevel(std::string(line_start, line_end));

                            // Set color based on log level
                            ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default color (white)
                            if (level == spdlog::level::err)
                                color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); // Red color for error messages
                            else if (level == spdlog::level::warn)
                                color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f); // Yellow color for warnings
                            else if (level == spdlog::level::info)
                                color = ImVec4(0.4f, 1.0f, 0.4f, 1.0f); // Green color for info messages

                            // Apply text color
                            ImGui::PushStyleColor(ImGuiCol_Text, color);
            
                            // Display the line
                            ImGui::TextUnformatted(line_start, line_end);

                    
            
                            // Pop the text color
                            ImGui::PopStyleColor();
                            
                        }
                    }
                    clipper.End();
                }
                ImGui::PopStyleVar();

                // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
                // Using a scrollbar or mouse-wheel will take away from the bottom edge.
                if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                
                // Restore the default font scale
                ImGui::GetFont()->Scale = oldFontSize;
                ImGui::PopFont();

              
            }
            ImGui::EndChild();
            ImGui::End();
        }
    };

    // Function to convert a time point to a string
    std::string timePointToString(const std::chrono::system_clock::time_point& time) {
        std::time_t timeT = std::chrono::system_clock::to_time_t(time);
        std::tm tm = *std::localtime(&timeT);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S"); // Customize the format if needed
        return oss.str();
    }

    class ImGuiConsoleSink : public spdlog::sinks::sink
    {
    public:
        ImGuiConsoleSink(const std::shared_ptr<ImGuiConsole>& console) : m_Console(console)
        {
            const int success = m_Console != nullptr;
            ENX_ENGINE_ASSERT(success, "ImGuiConsole is null!")

            m_Initialized = true;
        }

        ~ImGuiConsoleSink() = default;

        void log(const spdlog::details::log_msg& msg) override
        {
            if(!m_Initialized)
            {
                return;
            }

            // Convert time point to string
            auto timePoint = std::chrono::system_clock::to_time_t(msg.time);
            std::tm tm = *std::localtime(&timePoint);
            std::ostringstream timeStream;
            timeStream << std::put_time(&tm, "%H:%M:%S");
            
            // Build the log string
            // Build the log string
            const std::string logString = fmt::format("[{}] ({}) {}: {}\n",
                                                      timeStream.str(),
                                                      to_string_view(msg.level),
                                                      msg.logger_name,
                                                      msg.payload);
            
            

            m_Console->AddLog(logString.c_str());
        }
        void flush() override
        {
        
        }

        void set_pattern(const std::string& pattern) override
        {
            m_Pattern = pattern;
            spdlog::set_pattern(pattern, spdlog::pattern_time_type::local);   
        }
        void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override
        {
            
        }

    private:
        std::shared_ptr<ImGuiConsole> m_Console;
        bool m_Initialized = false;
        std::string m_Pattern;
    };
}
