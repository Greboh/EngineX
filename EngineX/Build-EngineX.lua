project "EngineX"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"

   targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

 -- Specify the PCH header file
    pchheader "enxpch.h"
    
 -- Specify the source file that includes the PCH header
    pchsource "Source/enxpch.cpp"

   files { "Source/**.h", "Source/**.cpp", "vendor/stb_image/**.h", "vendor/stb_image/**.cpp" }

    includedirs
    {
            "Source",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.spdlog}",
            "%{IncludeDir.stb_image}"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    links
    {
            "GLFW",
            "Glad",
            "ImGui",
            "opengl32.lib"
    }

   filter "system:windows"
       systemversion "latest"
        defines
        {
            "ENX_PLATFORM_WINDOWS",
            "ENX_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

   filter "configurations:Debug"
       defines { "DEBUG", "ENX_ENABLE_ASSERTS" }
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