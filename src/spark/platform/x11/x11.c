#include "spark/platform/platform.h"
#include <X11/Xlib.h>
#include <assert.h>
#include <stdlib.h>

spark_window *spark_create_window() {
	string8 display_name = string8_lit("Hello X11");
	Display *dpy = XOpenDisplay(NULL);
	dpy = NULL;
	if(dpy == NULL) {
		string8 err = string8_lit("An error occured: cannot open X display.\n");
		write(1, err.data, err.length);
		exit(-1);
	}
}
