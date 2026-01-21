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
	assert(a && "Arena must not be NULL");
	platform_window* win = arena_push(a, sizeof(*win));
	assert(win && "Failed to allocate platform_window");

	// Open X display
	win->dpy = XOpenDisplay(NULL);
	assert(win->dpy && "Failed to open X display");
	Window root = DefaultRootWindow(win->dpy);
	assert(root && "Failed to get default root window");

	// Choose FBConfig & Visual
	GLXFBConfig fb = choose_fbconfig(win->dpy, DefaultScreen(win->dpy));
	XVisualInfo* vi = glXGetVisualFromFBConfig(win->dpy, fb);
	assert(vi && "Failed to get XVisualInfo from FBConfig");

	// Create colormap
	XSetWindowAttributes attr = {0};
	attr.colormap = XCreateColormap(win->dpy, root, vi->visual, AllocNone);
	assert(attr.colormap && "Failed to create colormap");
	attr.event_mask = ExposureMask | KeyPressMask;

	// Set override_redirect if you want borderless / floating window
	// attr.override_redirect = True;

	// Create X window
	win->win = XCreateWindow(win->dpy,
	 root,
	 0,
	 0,
	 w,
	 h,
	 0,
	 vi->depth,
	 InputOutput,
	 vi->visual,
	 CWColormap | CWEventMask,
	 &attr);
	assert(win->win && "Failed to create X window");

	// --- Set size hints (force requested size) ---
	XSizeHints* size_hints = XAllocSizeHints();
	size_hints->flags = PSize | PMinSize | PMaxSize;
	size_hints->width = w;
	size_hints->height = h;
	size_hints->min_width = w;
	size_hints->min_height = h;
	size_hints->max_width = w;
	size_hints->max_height = h;
	XSetWMNormalHints(win->dpy, win->win, size_hints);
	XFree(size_hints);

	// --- Set window type to NORMAL ---
	Atom window_type = XInternAtom(win->dpy, "_NET_WM_WINDOW_TYPE", False);
	Atom normal_type = XInternAtom(win->dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);
	XChangeProperty(win->dpy,
	 win->win,
	 window_type,
	 XA_ATOM,
	 32,
	 PropModeReplace,
	 (unsigned char*)&normal_type,
	 1);

	// Set window title
	XStoreName(win->dpy, win->win, (const char*)title.data);

	// Map window
	XMapWindow(win->dpy, win->win);
	XFlush(win->dpy);

	// Wait for MapNotify to ensure the window is actually mapped
	XMapWindow(win->dpy, win->win);
	XFlush(win->dpy);
	// --- Create OpenGL context ---
	win->gl_context = create_core_context(win->dpy, fb);
	assert(win->gl_context && "Failed to create GL context");
	glXMakeCurrent(win->dpy, win->win, win->gl_context);

	// --- Initialize GLAD ---
	assert(gladLoadGL((GLADloadfunc)glXGetProcAddressARB) && "Failed to initialize GLAD");

	i32 major = 0, minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("OpenGL %d.%d loaded\n", major, minor);

	// Window & framebuffer sizes
	win->window_w = w;
	win->window_h = h;

	u32 fb_w = 0, fb_h = 0;
	glXQueryDrawable(win->dpy, win->win, GLX_WIDTH, &fb_w);
	glXQueryDrawable(win->dpy, win->win, GLX_HEIGHT, &fb_h);
	assert(fb_w > 0 && fb_h > 0 && "Framebuffer width/height invalid");

	win->fb_w = (i32)fb_w;
	win->fb_h = (i32)fb_h;

	// Set viewport
	glViewport(0, 0, win->fb_w, win->fb_h);

	return win;
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
