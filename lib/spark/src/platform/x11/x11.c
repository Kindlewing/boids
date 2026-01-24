#include "platform/x11.h"
#include <unistd.h>
#include "mem/arena.h"
#include "spark.h"
#include "str/string8.h"
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>

struct spark_window {
    Display *dpy;
    Window x_window;
    XEvent event;
    GLXFBConfig fb_cfg;
    XVisualInfo *vi;
    i32 scren_num;
    u32 width;
    u32 height;
};

static void x11_err(string8 err) {
    write(2, err.data, err.length);
}

static GLXFBConfig x11_get_framebuffer_config(Display *dpy, i32 screen) {
    i32 attributes[] = {
	    GLX_X_RENDERABLE,
	    True,
	    GLX_DRAWABLE_TYPE,
	    GLX_WINDOW_BIT,
	    GLX_RENDER_TYPE,
	    GLX_RGBA_BIT,
	    GLX_DOUBLEBUFFER,
	    True,
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
	    None,
    };
    i32 config_count = 0;

    // Try to get the configs
    GLXFBConfig *configs =
	    glXChooseFBConfig(dpy, screen, attributes, &config_count);

    if (config_count <= 0) {
	return NULL;
    }
    return configs[0];
}

spark_window *platform_create_window(arena *a, u32 w, u32 h, string8 title) {
    Display *dpy = XOpenDisplay(NULL);

    if (dpy == NULL) {
	x11_err(string8_lit("X Display could not be created\n"));
	return NULL;
    }
    spark_window *win = arena_push_struct_zero(a, spark_window);
    u32 screen = DefaultScreen(dpy);
    Window root = DefaultRootWindow(dpy);

    // Query the  GLX version
    i32 major;
    i32 minor;
    b8 ok = glXQueryVersion(win->dpy, &major, &minor);
    if (!ok) {
	x11_err(string8_lit("Unable to query the GLX version.\n"));
    }

    GLXFBConfig cfg = x11_get_framebuffer_config(dpy, screen);
    win->fb_cfg = x11_get_framebuffer_config(win->dpy, win->scren_num);
    if (win->fb_cfg == NULL) {
	x11_err(string8_lit("No suitible framebuffer config found.\n"));
	return NULL;
    }

    // Get XVisual
    XVisualInfo *vi = glXGetVisualFromFBConfig(dpy, cfg);
    XSetWindowAttributes attr = {0};
    i32 x = 0;
    i32 y = 0;
    i32 border_width = 0;
    attr.colormap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    attr.event_mask = ExposureMask | KeyPressMask;
    Window x_window = XCreateWindow(dpy, root, x, y, w, h, border_width,
				    vi->depth, vi->class, vi->visual,
				    CWEventMask | CWColormap, &attr);
    XMapWindow(win->dpy, win->x_window);
    XFlush(win->dpy);
    XStoreName(win->dpy, win->x_window, (char *)title.data);
    // TODO Create context, Initialize OpenGL/Glad

    win->dpy = dpy;
    win->width = w;
    win->height = h;
    win->vi = vi;
    win->fb_cfg = cfg;
    win->x_window = x_window;
    win->scren_num = screen;
    return win;
}

void platform_poll_events(spark_window *win) {
}

void platform_close_window(spark_window *win) {
    XUnmapWindow(win->dpy, win->x_window);
    XDestroyWindow(win->dpy, win->x_window);
}
