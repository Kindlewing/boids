#ifndef X11_H
#define X11_H
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glcorearb.h>
#include <assert.h>

GLXFBConfig x11_glx_choose_config(Display *dpy, int screen);
GLXContext x11_create_core_context(Display *dpy, GLXFBConfig fb);
#endif // X11_H
