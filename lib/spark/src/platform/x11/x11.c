#include "spark.h"
#include "mem/arena.h"
#include "str/string8.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "platform/x11.h"

struct spark_window {
	Display *dpy;
	Window x_window;
	XEvent event;
	u32 width;
	u32 height;
};

static i32 x11_err(string8 err, i32 code) {
	write(2, err.data, err.length);
	return code;
}

static GLXFBConfig x11_get_framebuffer_config(Display *dpy, i32 screen) {
	i32 attributes[] = {
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DOUBLEBUFFER, True,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
	};
}

spark_window *platform_create_window(arena *a, u32 w, u32 h, string8 title) {
	Display *dpy = XOpenDisplay(NULL);

	if(dpy == NULL) {
		return NULL;
	}
	spark_window *win = arena_push_struct_zero(a, spark_window);
	win->dpy = dpy;
	win->width = w;
	win->height = h;
	Window root = DefaultRootWindow(win->dpy);

	// Query the  GLX version
	i32 major;
	i32 minor;
	b8 ok = glXQueryVersion(win->dpy, &major, &minor);
	if(!ok) {
		x11_err(string8_lit("Unable to query the GLX version."), -1);
	}


	//TODO: Create non-simple X window.
	win->x_window = XCreateSimpleWindow(win->dpy, root, 0, 0, w, h, 0, 0, 0);
	XMapWindow(win->dpy, win->x_window);
	XFlush(win->dpy);
	XStoreName(win->dpy, win->x_window, (char *)title.data);
	// TODO Create context, Initialize OpenGL/Glad
	return win;
}

void platform_poll_events(spark_window *win) {
}

void platform_close_window(spark_window *win) {
	XUnmapWindow(win->dpy, win->x_window);
	XDestroyWindow(win->dpy, win->x_window);
}
