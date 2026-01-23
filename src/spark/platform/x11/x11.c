#include "base/string8.h"
#include "spark/platform/platform.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <assert.h>
#include <stdlib.h>
#include "spark/mem/internal_alloc.h"
#include "spark/spark.h"

struct spark_window {
	Display *dpy;
	Window win;
	XEvent event;
	u32 width;

	u32 height;
};

static GLXFBConfig choose_fbconfig(Display *dpy, int screen) {
	int attribs[] = { GLX_X_RENDERABLE,
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
		              None };
	int count;
	GLXFBConfig *configs = glXChooseFBConfig(dpy, screen, attribs, &count);
	assert(configs && count > 0);
	GLXFBConfig fb = configs[0];
	XFree(configs);
	return fb;
}

static spark_window *x11_create_window(u32 w, u32 h, string8 title) {
	Display *dpy = XOpenDisplay(NULL);
	if(dpy == NULL) {
		string8 err = string8_lit("An error occured: cannot open X display.\n");
		write(1, err.data, err.length);
		exit(-1);
	}
	spark_window *win = SPARK_ALLOC(spark_window);
	win->dpy = dpy;
	GLXFBConfig fb = choose_fbconfig(win->dpy, DefaultScreen(win->dpy));
	XVisualInfo *vi = glXGetVisualFromFBConfig(win->dpy, fb);
	assert(vi && "Failed to get XVisualInfo from FBConfig");
	XSetWindowAttributes attr = { 0 };
	Window root = DefaultRootWindow(win->dpy);
	attr.colormap = XCreateColormap(win->dpy, root, vi->visual, AllocNone);
	attr.event_mask = ExposureMask | KeyPressMask;
	win->win = XCreateWindow(
	 win->dpy, root, 0, 0, w, h, 0, vi->depth, InputOutput, vi->visual,
	 CWColormap | CWEventMask, &attr);
	XMapWindow(win->dpy, win->win);
	XFlush(win->dpy);
	XStoreName(win->dpy, win->win, (char *)title.data);
	// TODO Create context, Initialize OpenGL/Glad
	return win;
}

static void x11_poll_events(spark_window *win) {
	while(XPending(win->dpy)) { // check for queued events
		XNextEvent(win->dpy, &win->event);
		switch(win->event.type) {
		case Expose:
			// redraw here
			break;

		case KeyPress: {
			KeySym key = XLookupKeysym(&win->event.xkey, 0);
			if(key == XK_Escape) {
				XDestroyWindow(win->dpy, win->win);
				XCloseDisplay(win->dpy);
				exit(0);
			}
			break;
		}
		}
	}
}

spark_platform_api spark_platform = {
	.create_window = x11_create_window,
	.poll_events = x11_poll_events,
};
