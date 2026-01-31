project("base")
kind("StaticLib")

targetdir("../../build/lib/%{cfg.buildcfg}")
objdir("../../build/obj/%{prj.name}/%{cfg.buildcfg}")

files({
	"src/string8.c",
	"src/math.c",
	"src/profile.c",
	"include/**.h",
})

includedirs({
	"include",
})

filter("system:windows")
files({ "src/arena_win32.c" })

filter("system:linux")
files({ "src/arena_linux.c" })

filter("toolset:gcc or toolset:clang")
buildoptions({ "-Wall", "-Wextra" })

filter({})
