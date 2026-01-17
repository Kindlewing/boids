#include "platform.h"
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct platform_window {
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

bool platform_init(void) {
	Display *dpy = XOpenDisplay(NULL);
	if(!dpy) {
		fprintf(stderr, "Platform init: Failed to open X display\n");
		return false;
	}

	// 2️⃣ Pick default screen
	int screen = DefaultScreen(dpy);

	// 3️⃣ Check for a suitable GLX visual
	int visual_attribs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
	XVisualInfo *vi = glXChooseVisual(dpy, screen, visual_attribs);
	if(!vi) {
		fprintf(stderr, "Platform init: No suitable GLX visual found\n");
		XCloseDisplay(dpy);
		return false;
	}

	// 4️⃣ Check framebuffer config availability
	int fb_attribs[] = {
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
	int fb_count = 0;
	GLXFBConfig *fb_configs = glXChooseFBConfig(dpy, screen, fb_attribs, &fb_count);
	if(!fb_configs || fb_count == 0) {
		fprintf(stderr, "Platform init: No suitable GLX framebuffer config\n");
		if(fb_configs)
			XFree(fb_configs);
		XFree(vi);
		XCloseDisplay(dpy);
		return false;
	}
	XFree(fb_configs);
	XFree(vi);

	// 5️⃣ Optional: check OpenGL 4.6 context creation
	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
	    (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB(
	        (const GLubyte *)"glXCreateContextAttribsARB");

	if(!glXCreateContextAttribsARB) {
		fprintf(stderr, "Platform init: GLX_ARB_create_context not supported\n");
		XCloseDisplay(dpy);
		return false;
	}

	// 6️⃣ Close display, this is just preflight
	XCloseDisplay(dpy);

	return true; // Everything looks good
}

platform_window *platform_create_window(arena *a, int w, int h, const char *title) {
	assert(a && "Arena must not be NULL");

	platform_window *win = arena_alloc(a, sizeof(*win));
	assert(win && "Failed to allocate platform_window");

	win->dpy = XOpenDisplay(NULL);
	assert(win->dpy && "Failed to open X display");

	Window root = DefaultRootWindow(win->dpy);

	GLXFBConfig fb = choose_fbconfig(win->dpy, DefaultScreen(win->dpy));
	XVisualInfo *vi = glXChooseVisual(win->dpy, 0,
	                                  (int[]){GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None});

	XSetWindowAttributes attr = {0};
	attr.colormap = XCreateColormap(win->dpy, root, vi->visual, AllocNone);
	attr.event_mask = ExposureMask | KeyPressMask;

	win->win = XCreateWindow(win->dpy, root, 0, 0, w, h, 0, vi->depth, InputOutput,
	                         vi->visual, CWColormap | CWEventMask, &attr);

	XMapWindow(win->dpy, win->win);
	XStoreName(win->dpy, win->win, title);

	win->gl_context = create_core_context(win->dpy, fb);
	glXMakeCurrent(win->dpy, win->win, win->gl_context);

	return win;
}

void platform_poll_events(platform_window *win, bool *should_close) {
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

void platform_destroy_window(platform_window *win) {
	glXMakeCurrent(win->dpy, None, NULL);
	glXDestroyContext(win->dpy, win->gl_context);
	XDestroyWindow(win->dpy, win->win);
	XCloseDisplay(win->dpy);
}
