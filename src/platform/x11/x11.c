#include "base/typedefs.h"
#include "platform/platform.h"
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <assert.h>
#include <stdio.h>
#include <third_party/glad/gl.h>

struct platform_window {
	platform_state state;

	i32 window_w;
	i32 window_h;

	i32 fb_w;
	i32 fb_h;

	Display* dpy;
	Window win;
	GLXContext gl_context;
};

static GLXFBConfig choose_fbconfig(Display* dpy, i32 screen) {
	assert(dpy && "Display must not be NULL");

	i32 attribs[] = {GLX_X_RENDERABLE,
	 True,
	 GLX_DRAWABLE_TYPE,
	 GLX_WINDOW_BIT,
	 GLX_RENDER_TYPE,
	 GLX_RGBA_BIT,
	 GLX_X_VISUAL_TYPE,
	 GLX_TRUE_COLOR,
	 GLX_RED_SIZE,
	 8,
	 GLX_GREEN_SIZE,
	 8,
	 GLX_BLUE_SIZE,
	 8,
	 GLX_ALPHA_SIZE,
	 8,
	 GLX_DEPTH_SIZE,
	 24,
	 GLX_STENCIL_SIZE,
	 8,
	 GLX_DOUBLEBUFFER,
	 True,
	 None};

	i32 count = 0;
	GLXFBConfig* configs = glXChooseFBConfig(dpy, screen, attribs, &count);
	assert(configs && count > 0 && "No valid FBConfig found");

	GLXFBConfig fb = configs[0];
	XFree(configs);
	return fb;
}

static GLXContext create_core_context(Display* dpy, GLXFBConfig fb) {
	assert(dpy && "Display must not be NULL");

	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
	 (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB(
	  (const GLubyte*)"glXCreateContextAttribsARB");
	assert(glXCreateContextAttribsARB && "GLX_ARB_create_context not supported");

	i32 attribs[] = {GLX_CONTEXT_MAJOR_VERSION_ARB,
	 4,
	 GLX_CONTEXT_MINOR_VERSION_ARB,
	 6,
	 GLX_CONTEXT_PROFILE_MASK_ARB,
	 GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifndef NDEBUG
	 GLX_CONTEXT_FLAGS_ARB,
	 GLX_CONTEXT_DEBUG_BIT_ARB,
#endif
	 None};

	GLXContext ctx = glXCreateContextAttribsARB(dpy, fb, 0, True, attribs);
	assert(ctx && "Failed to create OpenGL 4.6 core context");
	return ctx;
}

platform_window* platform_create_window(arena* a, u32 w, u32 h, string8 title) {
}

void platform_poll_events(platform_window* win, b8* should_close) {
	assert(win && "platform_window is NULL");
	assert(win->dpy && "Display is NULL");
	assert(should_close && "should_close pointer is NULL");

	XEvent event;
	while(XPending(win->dpy)) {
		XNextEvent(win->dpy, &event);
		if(event.type == KeyPress)
			*should_close = true;
	}
}

void platform_swap_buffers(platform_window* win) {
	assert(win && "platform_window is NULL");
	assert(win->dpy && "Display is NULL");
	assert(win->win && "Window is NULL");

	glXSwapBuffers(win->dpy, win->win);
}

void platform_close_window(platform_window* win) {
	if(!win)
		return;
	if(win->dpy) {
		if(win->gl_context)
			glXMakeCurrent(win->dpy, None, NULL);
		if(win->gl_context)
			glXDestroyContext(win->dpy, win->gl_context);
		if(win->win)
			XDestroyWindow(win->dpy, win->win);
		XCloseDisplay(win->dpy);
	}
}
