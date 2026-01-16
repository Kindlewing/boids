#ifndef X11_H
#define X11_H
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glcorearb.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "typedefs.h"

GLXFBConfig x11_glx_choose_config(Display *dpy, int screen);
GLXContext x11_create_core_context(Display *dpy, GLXFBConfig fb);
void x11_init(void);
#endif // X11_H
