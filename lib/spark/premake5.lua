project("spark")
kind("StaticLib")

targetdir("../../build/lib/%{cfg.buildcfg}")
objdir("../../build/obj/%{prj.name}/%{cfg.buildcfg}")

files({
	"src/spark.c",
	"src/render/shader.c",
	"src/glad/glad.c",
	"include/**.h",
})

includedirs({
	"include",
	"../base/include",
})

links({
	"base",
})

-- ========================
-- Platform-specific sources
-- ========================
filter("system:windows")
files({ "src/platform/win32/win32.c" })
links({ "opengl32" })

filter("system:linux")
files({ "src/platform/x11/x11.c" })
links({
	"GL",
	"X11",
})

filter("system:macosx")
files({ "src/platform/cocoa/cocoa.m" })
links({
	"OpenGL.framework",
	"Cocoa.framework",
})

filter("toolset:gcc or toolset:clang")
buildoptions({ "-Wall", "-Wextra" })

filter({})
