#include "enxpch.h"
#include "enxpch.h"
#include "ImGuiConsoleWidget.h"

#include "imgui.h"

namespace EngineX
{
    void ImGuiConsole::Clear()
    {
        m_TextBuffer.clear();
        m_LineOffsets.clear();
        m_LineOffsets.push_back(0);
    }

    void ImGuiConsole::AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = m_TextBuffer.size();
        va_list args;
        va_start(args, fmt);
        m_TextBuffer.appendfv(fmt, args);
        va_end(args);
        for (int new_size = m_TextBuffer.size(); old_size < new_size; old_size++)
            if (m_TextBuffer[old_size] == '\n')
                m_LineOffsets.push_back(old_size + 1);
    }

    void ImGuiConsole::Draw(const char* title, bool* p_open, ImGuiWindowFlags flags)
    {
        if (!ImGui::Begin(title, p_open, flags))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
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
        m_TextFilter.Draw("Filter", -100.0f);

        ImGui::Separator();

        if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (clear)
                Clear();
            if (copy)
                ImGui::LogToClipboard();

            // Increase the font scale to make the text bigger
            const float oldFontSize = ImGui::GetFont()->Scale;
            ImGui::GetFont()->Scale *= 1.0f;
            ImGui::PushFont(ImGui::GetFont());
                
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 7.5));
            const char* buf = m_TextBuffer.begin();
            const char* buf_end = m_TextBuffer.end();
            if (m_TextFilter.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have random access to the result of our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < m_LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + m_LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < m_LineOffsets.Size)
                                               ? (buf + m_LineOffsets[line_no + 1] - 1)
                                               : buf_end;
                    if (m_TextFilter.PassFilter(line_start, line_end))
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
                clipper.Begin(m_LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + m_LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < m_LineOffsets.Size)
                                                   ? (buf + m_LineOffsets[line_no + 1] - 1)
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
            if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
                
            // Restore the default font scale
            ImGui::GetFont()->Scale = oldFontSize;
            ImGui::PopFont();
        }
        ImGui::EndChild();
        ImGui::End();
    }    
}
