#include <EGL/egl.h>
#define eglUpdateDisplay(state) eglSwapBuffers((state)->display, (state)->surface)

typedef struct {
    EGLint x, y, width, height;
} EGLRect;

typedef struct {
    EGLRect    win;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
} EGLDisplayState;

EGLNativeWindowType eglNativeWindow(char *winTitle, EGLint *winWidth, EGLint *winHeight, EGLint winFlags);
EGLDisplayState *eglOpenDisplay(EGLint winMinSize, EGLint attribColorSize, EGLint attribAlphaSize, EGLint attribZDepth, EGLint attribRenderType);
void eglCloseDisplay(EGLDisplayState *state);
