-- premake5.lua
workspace "EngineX"
   architecture "x64"
   configurations { "Debug", "Staging", "Release" }
   startproject "Sandbox"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "EngineX"
	include "EngineX/Build-EngineX.lua"
group "Sandbox"
   include "Sandbox/Build-Sandbox.lua"