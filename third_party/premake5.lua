-- third_party/premake5.lua
project "ThirdParty"
    location (path.join(BuildRoot, "third_party"))
    kind "StaticLib"

    files {
        "**/**.h",
        "**/**.cpp",
        "**/**.inl"
    }

    includedirs { "third_party" }
    debugdir(_MAIN_SCRIPT_DIR)