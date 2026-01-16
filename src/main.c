#include <GL/glx.h>
#include <stdlib.h>
#include <stdio.h>
#include "typedefs.h"
#include "x11.h"

int main(void) {
    GLXContext gl_context = {0};
    XWindowAttributes window_attribs = {0};
    XEvent xevent = {0};

    x11_init();
    printf("About to get GLX context\n");
    GLXFBConfig fb = x11_glx_choose_config(display, DefaultScreen(display));
    gl_context = x11_create_core_context(display, fb);

    glXMakeContextCurrent(display, window, window, gl_context);
    if(glXGetCurrentContext() != gl_context) {
        fprintf(stderr, "Failed to make OpenGL context current\n");
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);

    bool window_should_close = false;
    while(!window_should_close) {
        XNextEvent(display, &xevent);
        if(xevent.type == KeyPress) {
            window_should_close = true;
        }
        XGetWindowAttributes(display, window, &window_attribs);
        glClearColor(30.0 / 255.0, 30.0 / 255.0, 46.0 / 255.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window_attribs.width, window_attribs.height);
        glXSwapBuffers(display, window);
    }
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, gl_context);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}
