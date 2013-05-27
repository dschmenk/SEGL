#include <stdio.h>
#include <math.h>
#include <VG/openvg.h>
#include <VG/vgu.h>
#include "segl.h"

int main(int argc, char **argv)
{
    float rotate, color[4] = {0.0, 0.33, 0.33, 1.0};
    VGPath path;
    VGPaint paintStroke;
    VGubyte pathSegments[] = {VG_MOVE_TO_ABS, VG_LINE_TO_ABS, VG_CLOSE_PATH};
    float   pathData[] = {-100.0, -100.0, 100.0, 100.0};
    EGLDisplayState *eglState = eglOpenDisplay(0 /* min win dimension */,
                                               8 /* min color component size */,
                                               8 /* min alpha size */,
                                               0 /* min Z depth */,
                                               EGL_OPENVG_BIT /* render type */);
    path = vgCreatePath(VG_PATH_FORMAT_STANDARD,
                        VG_PATH_DATATYPE_F,
                        1.0, 0.0,
                        0,   0,
                        VG_PATH_CAPABILITY_APPEND_TO);
    vgAppendPathData(path, 3, pathSegments, pathData);
    paintStroke = vgCreatePaint();
    vgSetColor(paintStroke, 0xFF0000FF);
    vgSetParameteri(paintStroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetPaint(paintStroke, VG_STROKE_PATH | VG_FILL_PATH);
    vgSetf(VG_STROKE_LINE_WIDTH, 5.0);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    for (rotate = 0.0; rotate < 360.0; rotate++) {
        color[0] = rotate / 360.0;
        vgSetfv(VG_CLEAR_COLOR, 4, color);
        vgClear(0, 0, eglState->win.width, eglState->win.height);
        vgLoadIdentity();
        vgTranslate(eglState->win.width/2, eglState->win.height/2);
        vgRotate(rotate);
        vgDrawPath(path, VG_STROKE_PATH);
        eglUpdateDisplay(eglState);
    }
    eglCloseDisplay(eglState);
}
