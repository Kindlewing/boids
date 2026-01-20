#include "platform/platform.h"
#include <X11/Xlib.h>
#include <assert.h>
#include <GL/glx.h>
#include <stdio.h>

struct platform_window {
	platform_state state;

	Display *dpy;
	Window win;
	GLXContext gl_context;
};

static GLXFBConfig choose_fbconfig(Display *dpy, int screen) {
	int attribs[] = {
	    GLX_X_RENDERABLE, True,
	    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
	    GLX_RENDER_TYPE, GLX_RGBA_BIT,
	    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
	    GLX_RED_SIZE, 8,
	    GLX_GREEN_SIZE, 8,
	    GLX_BLUE_SIZE, 8,
	    GLX_ALPHA_SIZE, 8,
	    GLX_DEPTH_SIZE, 24,
	    GLX_STENCIL_SIZE, 8,
	    GLX_DOUBLEBUFFER, True,
	    None};

	int count;
	GLXFBConfig *configs = glXChooseFBConfig(dpy, screen, attribs, &count);
	assert(configs && count > 0);
	GLXFBConfig fb = configs[0];
	XFree(configs);
	return fb;
}

static GLXContext create_core_context(Display *dpy, GLXFBConfig fb) {
	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
	    (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB(
	        (const GLubyte *)"glXCreateContextAttribsARB");

	assert(glXCreateContextAttribsARB && "GLX_ARB_create_context not supported");

	int attribs[] = {
	    GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
	    GLX_CONTEXT_MINOR_VERSION_ARB, 6,
	    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifndef NDEBUG
	    GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
#endif
	    None};

	GLXContext ctx = glXCreateContextAttribsARB(dpy, fb, 0, True, attribs);
	assert(ctx && "Failed to create OpenGL 4.6 core context");
	return ctx;
}

platform_window *platform_create_window(arena *a, int w, int h, const char *title) {
	assert(a && "Arena must not be NULL");

	platform_window *win = arena_push(a, sizeof(*win));
	assert(win && "Failed to allocate platform_window");

	win->dpy = XOpenDisplay(NULL);
	assert(win->dpy && "Failed to open X display");

	Window root = DefaultRootWindow(win->dpy);
	GLXFBConfig fb = choose_fbconfig(win->dpy, DefaultScreen(win->dpy));
	XVisualInfo *vi = glXGetVisualFromFBConfig(win->dpy, fb);
	assert(vi && "Failed to get XVisualInfo from FBConfig");

	XSetWindowAttributes attr = {0};
	attr.colormap = XCreateColormap(win->dpy, root, vi->visual, AllocNone);
	attr.event_mask = ExposureMask | KeyPressMask;

	win->win = XCreateWindow(win->dpy, root, 0, 0, w, h, 0, vi->depth, InputOutput,
	                         vi->visual, CWColormap | CWEventMask, &attr);

	XMapWindow(win->dpy, win->win);
	XStoreName(win->dpy, win->win, title);

	// Create the OpenGL context
	win->gl_context = create_core_context(win->dpy, fb);
	glXMakeCurrent(win->dpy, win->win, win->gl_context);

	// --- Initialize GLAD here ---
	// cast glXGetProcAddressARB to GLADloadproc
	if(!gladLoadGL((GLADloadfunc)glXGetProcAddressARB)) {
		fprintf(stderr, "Failed to initialize GLAD\n");
		platform_close_window(win);
		return NULL;
	}
	i32 major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("OpenGL %d.%d loaded\n", major, minor);
	return win;
}

void platform_poll_events(platform_window *win, b8 *should_close) {
	XEvent event;
	while(XPending(win->dpy)) {
		XNextEvent(win->dpy, &event);
		if(event.type == KeyPress)
			*should_close = true;
	}
}

void platform_swap_buffers(platform_window *win) {
	glXSwapBuffers(win->dpy, win->win);
}

void platform_close_window(platform_window *win) {
	glXMakeCurrent(win->dpy, None, NULL);
	glXDestroyContext(win->dpy, win->gl_context);
	XDestroyWindow(win->dpy, win->win);
	XCloseDisplay(win->dpy);
}
