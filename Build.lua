include "Dependencies.lua"

workspace "EngineX"
   architecture "x64"
   configurations { "Debug", "Staging", "Release" }
   startproject "Sandbox"
   flags { "MultiProcessorCompile" }

   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "EngineX"
	include "EngineX/Build-EngineX.lua"

group "Sandbox"
   include "Sandbox/Build-Sandbox.lua"

group "Dependencies"
      include "EngineX/vendor/GLFW/Build-GLFW.lua"
      include "EngineX/vendor/Glad/Build-Glad.lua"