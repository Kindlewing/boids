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

static spark_window *x11_create_window(u32 w, u32 h, string8 title) {
	Display *dpy = XOpenDisplay(NULL);
	if(dpy == NULL) {
		string8 err = string8_lit("An error occured: cannot open X display.\n");
		write(1, err.data, err.length);
		exit(-1);
	}
	spark_window *win = SPARK_ALLOC(spark_window);
	win->dpy = dpy;
	Window root = DefaultRootWindow(win->dpy);
	win->win = XCreateSimpleWindow(win->dpy, root, 0, 0, w, h, 0, 0, 0);
	XMapWindow(win->dpy, win->win);
	XFlush(win->dpy);
	XStoreName(win->dpy, win->win, (char *)title.data);
	// TODO Create context, Initialize OpenGL/Glad
	return win;
}

static void x11_poll_events(spark_window *win) {
}

static void x11_close_window(spark_window *win) {
}

spark_platform_api spark_platform = { .create_window = x11_create_window,
	                                  .poll_events = x11_poll_events };
