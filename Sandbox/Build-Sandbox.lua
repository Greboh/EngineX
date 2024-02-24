project "Sandbox"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",
      "%{wks.location}/EngineX/vendor/spdlog/include",
      "%{wks.location}/EngineX/vendor/ImGui",
      "%{wks.location}/EngineX/vendor/glm",
      "%{wks.location}/EngineX/Source",
   }

   links
   {
      "EngineX"
   }

   defines
   {
        "ASSETS_DIR=\"%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}/Source/EngineX/Assets/\""
   }

   targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines
       {
           "WINDLL",
           "ENX_PLATFORM_WINDOWS",
       }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Staging"
       defines { "STAGING" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "Off"