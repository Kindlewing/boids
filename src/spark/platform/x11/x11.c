#include "base/arena.h"
#include "base/string8.h"
#include "spark/spark.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <assert.h>
#include <stdlib.h>
#include "spark/platform/x11.h"

struct spark_window {
	Display *dpy;
	Window win;
	XEvent event;
	u32 width;
	u32 height;
};

spark_window *platform_create_window(arena *a, u32 w, u32 h, string8 title) {
	Display *dpy = XOpenDisplay(NULL);

	if(dpy == NULL) {
		string8 err = string8_lit("An error occured: cannot open X display.\n");
		write(1, err.data, err.length);
		exit(-1);
	}
	spark_window *win = arena_push_struct_zero(a, spark_window);
	win->dpy = dpy;
	Window root = DefaultRootWindow(win->dpy);
	win->win = XCreateSimpleWindow(win->dpy, root, 0, 0, w, h, 0, 0, 0);
	XMapWindow(win->dpy, win->win);
	XFlush(win->dpy);
	XStoreName(win->dpy, win->win, (char *)title.data);
	// TODO Create context, Initialize OpenGL/Glad
	return win;
}

void platform_poll_events(spark_window *win) {
}

void platform_close_window(spark_window *win) {
}
