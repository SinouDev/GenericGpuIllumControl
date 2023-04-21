-- premake5.lua
workspace "GenericGpuIllumControl"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "GenericGpuIllumControl"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "GenericGpuIllumControl"