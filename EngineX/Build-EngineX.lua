project "EngineX"
   kind "SharedLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"

   targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../bin-int/" .. OutputDir .. "/%{prj.name}")

 -- Specify the PCH header file
    pchheader "enxpch.h"
    
 -- Specify the source file that includes the PCH header
    pchsource "Source/enxpch.cpp"

   files { "Source/**.h", "Source/**.cpp" }

    includedirs
    {
            "Source",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.spdlog}"
    }

    links
    {
            "GLFW",
            "Glad",
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
        postbuildcommands
        {
             ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. OutputDir .. "/Sandbox")
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