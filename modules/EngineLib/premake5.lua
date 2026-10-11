-- modules/EngineLib/premake5.lua
project "EngineLib"
    location (path.join(BuildRoot, "EngineLib"))
    kind "StaticLib"
    rtti "On"

    files {
        "include/**.h",
        "include/**.hpp",
        "include/**.inl",
        "src/**.cpp",
        path.join(_MAIN_SCRIPT_DIR, "resources/Shaders/**.hlsl")
    }

    includedirs { 
        "include",
        path.join(_MAIN_SCRIPT_DIR, "third_party")
    }
    links { "ThirdParty" }

    -- 현재 엔진은 실행 중에 HLSL을 컴파일
    filter "files:**.hlsl"
        buildaction "None"
    filter {}

    debugdir(_MAIN_SCRIPT_DIR)