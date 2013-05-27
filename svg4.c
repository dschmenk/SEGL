#include <stdio.h>
#include <math.h>
#include <VG/openvg.h>
#include <VG/vgu.h>
#include "segl.h"
#define IMAGE_SIZE 64
int main(int argc, char **argv)
{
    int i;
    float rotate, color[4] = {0.0, 0.33, 0.33, 1.0}, fade[4]={1.0, 0.0, 0.0, 1.0};
    VGImage image;
    EGLDisplayState *eglState = eglOpenDisplay(0 /* min win dimension */,
                                               8 /* min color component size */,
                                               8 /* min alpha size */,
                                               0 /* min Z depth */,
                                               EGL_OPENVG_BIT /* render type */);
    image = vgCreateImage(VG_lRGBA_8888, IMAGE_SIZE, IMAGE_SIZE, VG_IMAGE_QUALITY_FASTER);
    // Fill image with a pattern
    for (i = 0; i < IMAGE_SIZE/2; i++) {
    fade[2] = (float)i / IMAGE_SIZE/2;
        fade[1] = (float)(i & 1);
        vgSetfv(VG_CLEAR_COLOR, 4, fade);
    vgClearImage(image, i, i, IMAGE_SIZE - 2 * i, IMAGE_SIZE - 2 * i);
    }
    vgSeti(VG_IMAGE_MODE, VG_DRAW_IMAGE_NORMAL);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    // Rotate image in center of screen
    for (rotate = 0.0; rotate < 360.0; rotate++) {
        color[0] = rotate / 360.0;
        vgSetfv(VG_CLEAR_COLOR, 4, color);
        vgClear(0, 0, eglState->win.width, eglState->win.height);
        vgLoadIdentity();
    vgTranslate(eglState->win.width/2, eglState->win.height/2);
    vgRotate(rotate);
    vgTranslate(-IMAGE_SIZE/2, -IMAGE_SIZE/2);
    vgDrawImage(image);
        eglUpdateDisplay(eglState);
    }
    eglCloseDisplay(eglState);
}
