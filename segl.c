#include <bcm_host.h>
#include "segl.h"

EGLNativeWindowType eglNativeWindow(char *winTitle, EGLint *winWidth, EGLint *winHeight, EGLint winFlags)
{
   static EGL_DISPMANX_WINDOW_T nativeWindow;
   DISPMANX_ELEMENT_HANDLE_T    element;
   DISPMANX_DISPLAY_HANDLE_T    display;
   DISPMANX_MODEINFO_T          mode;
   DISPMANX_UPDATE_HANDLE_T     update;
   VC_RECT_T                    rectDst, rectSrc;
   VC_DISPMANX_ALPHA_T          alpha = {DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS, 
                                         255, 
                                         0};

   bcm_host_init();
   display = vc_dispmanx_display_open(0);
   update  = vc_dispmanx_update_start(0);
   vc_dispmanx_display_get_info(display, &mode);
   if (*winHeight == 0 && *winWidth == 0)
      *winHeight = 480;
   if (*winHeight == 0)
      *winHeight = *winWidth * mode.height / mode.width;
   if (*winWidth == 0)
      *winWidth = *winHeight * mode.width / mode.height;
   rectDst.x            = 0;
   rectDst.y            = 0;
   rectDst.width        = mode.width;
   rectDst.height       = mode.height;
   rectSrc.x            = 0;
   rectSrc.y            = 0;
   rectSrc.width        = *winWidth  << 16;
   rectSrc.height       = *winHeight << 16;
   nativeWindow.width   = *winWidth;
   nativeWindow.height  = *winHeight;
   nativeWindow.element = vc_dispmanx_element_add(update, 
						  display, 
						  0 /* layer */, 
						  &rectDst,
						  0 /* src */,
						  &rectSrc,
						  DISPMANX_PROTECTION_NONE,
						  &alpha /* alpha */,
						  0 /* clamp */,
						  0 /* xform */);
   vc_dispmanx_update_submit_sync(update);
   vc_dispmanx_display_close(display);
   return (EGLNativeWindowType) &nativeWindow;
}
EGLDisplayState *eglOpenDisplay(EGLint winMinSize, EGLint attribColorSize, EGLint attribAlphaSize, EGLint attribZDepth, EGLint attribRenderType)
{
   static EGLDisplayState state;
   EGLint attribList[] = {
      EGL_RED_SIZE,        8,
      EGL_GREEN_SIZE,      8,
      EGL_BLUE_SIZE,       8,
      EGL_ALPHA_SIZE,      8,
      EGL_DEPTH_SIZE,      0,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
      EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
      EGL_NONE
   };
   EGLint    num;
   EGLConfig config;

   if (attribRenderType & EGL_OPENVG_BIT)
      eglBindAPI(EGL_OPENVG_API);
   else
      eglBindAPI(EGL_OPENGL_ES_API);
   attribList[1]    = attribList[3] = attribList[5] = attribColorSize;
   attribList[7]    = attribAlphaSize;
   attribList[9]    = attribZDepth;
   attribList[11]   = attribRenderType;
   state.win.x      = 0;
   state.win.y      = 0;
   state.win.width  = 0;
   state.win.height = winMinSize;
   state.display    = eglGetDisplay(EGL_DEFAULT_DISPLAY);
   eglInitialize(state.display, NULL, NULL);
   eglChooseConfig(state.display, attribList, &config, 1, &num);
   state.context = eglCreateContext(state.display, config, EGL_NO_CONTEXT, NULL);
   state.surface = eglCreateWindowSurface(state.display, config, eglNativeWindow(NULL, &state.win.width, &state.win.height, 0), NULL);
   eglMakeCurrent(state.display, state.surface, state.surface, state.context);
   return &state;
}
void eglCloseDisplay(EGLDisplayState *state)
{
   eglMakeCurrent(state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
   eglDestroySurface(state->display, state->surface);
   eglDestroyContext(state->display, state->context);
   eglTerminate(state->display);
}
