#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glcorearb.h>
#include <assert.h>
#include <stdlib.h>
#include "x11.h"

GLXFBConfig x11_glx_choose_config(Display *dpy, int screen) {
    int attribs[] = {GLX_X_RENDERABLE,
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
                     None};

    int count = 0;
    GLXFBConfig *configs = glXChooseFBConfig(dpy, screen, attribs, &count);

    assert(configs && count > 0);

    GLXFBConfig result = configs[0];
    XFree(configs);
    return result;
}

GLXContext x11_create_core_context(Display *dpy, GLXFBConfig fb) {
    PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
        (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB(
            (const GLubyte *)"glXCreateContextAttribsARB");

    assert(glXCreateContextAttribsARB &&
           "GLX_ARB_create_context not supported");

    int attribs[] = {GLX_CONTEXT_MAJOR_VERSION_ARB,
                     4,
                     GLX_CONTEXT_MINOR_VERSION_ARB,
                     5,
                     GLX_CONTEXT_PROFILE_MASK_ARB,
                     GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifndef NDEBUG
                     GLX_CONTEXT_FLAGS_ARB,
                     GLX_CONTEXT_DEBUG_BIT_ARB,
#endif
                     None};

    GLXContext ctx = glXCreateContextAttribsARB(dpy, fb,
                                                0,    // no sharing
                                                True, // direct rendering
                                                attribs);

    assert(ctx && "Failed to create OpenGL 4.5 core context");
    return ctx;
}

void x11_init(void) {
    XSetWindowAttributes win_attribs = {0};
    Window window = {0};
    Display *display = {0};

    display = XOpenDisplay(NULL);
    if(display == NULL) {
        perror("Error connecting to X11 server");
        exit(EXIT_FAILURE);
    }

    int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    Window root = DefaultRootWindow(display);
    XVisualInfo *vi = glXChooseVisual(display, 0, attributes);
    if(vi == NULL) {
        printf("No suitable visual found\n");
        exit(0);
    }
    printf("Visual %p selected\n", (void *)vi->visualid);
    Colormap colormap = XCreateColormap(display, root, vi->visual, AllocNone);
    win_attribs.colormap = colormap;
    win_attribs.event_mask = ExposureMask | KeyPressMask;
    u32 x = 0;
    u32 y = 0;
    u32 width = 800;
    u32 height = 800;
    u32 border_width = 0;
    window = XCreateWindow(display, root, x, y, width, height, border_width,
                           vi->depth, InputOutput, vi->visual,
                           CWColormap | CWEventMask, &win_attribs);
    XMapWindow(display, window);
    XStoreName(display, window, "GUI");
}
