project "EngineX"
   kind "SharedLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"

   targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../bin-int/" .. OutputDir .. "/%{prj.name}")

   pchheader "enxpch.h"
   pchsource "Source/enxpch.h"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
        "Source",
        "vendor/spdlog/include"
   }

   filter "system:windows"
       systemversion "latest"
        defines
        {
            "WINDLL",  
            "ENX_PLATFORM_WINDOWS",
            "ENX_BUILD_DLL"
        }
        postbuildcommands
        {
             ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. OutputDir .. "/Sandbox")
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