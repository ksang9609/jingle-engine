-- 저장소 루트: premake5.lua
BuildRoot = path.join(_MAIN_SCRIPT_DIR, "build")

workspace "JungleEngine"
    location (BuildRoot)
    configurations { "Debug", "Release"}
    platforms { "x64" }
    startproject "EngineApp"

    system "windows"
    systemversion "latest"
    architecture "x86_64"

    language "C++"
    cppdialect "C++23"
    characterset "Unicode"
    staticruntime "Off"
    warnings "Extra"

    multiprocessorcompile "On"

    -- 실행 파일/라이브러리와 중간 파일을 분리
    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    objdir "bin-int/%{prj.name}/%{cfg.platform}/%{cfg.buildcfg}"

    filter "configurations:Debug"
        defines { "_DEBUG" }
        runtime "Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        symbols "Off"
        optimize "Speed"

    filter {}


-- Include project premake5.lua files
-- Apps
include "apps/EngineApp/premake5.lua"

-- Modules
include "modules/EngineLib/premake5.lua"

-- Tests
include "tests/premake5.lua"

-- Third Party
include "third_party/premake5.lua"