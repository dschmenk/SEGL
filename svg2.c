#include <stdio.h>
#include <math.h>
#include <VG/openvg.h>
#include <VG/vgu.h>
#include "segl.h"

void bounce(int numpts, float *pos, float *inc, float width, float height)
{
    while (numpts--) {
        *pos += *inc;
        if (*pos >= width) {
            *pos = width - (*pos - width);
            *inc = -*inc;
        }
        if (*pos < 0.0) {
            *pos = -*pos;
            *inc = -*inc;
        }
        pos++;
        inc++;
        *pos += *inc;
        if (*pos >= height) {
            *pos = height - (*pos - height);
            *inc = -*inc;
        }
        if (*pos < 0.0) {
            *pos = -*pos;
            *inc = -*inc;
        }
        pos++;
        inc++;
    }
}

int main(int argc, char **argv)
{
    float rotate, color[4] = {0.0, 0.33, 0.33, 1.0};
    VGPath path;
    VGPaint paintStroke;
    VGubyte pathSegments[] = {VG_MOVE_TO_ABS, VG_LINE_TO_ABS, VG_CLOSE_PATH};
    float   pathData[] = {200.0, 300.0, 100.0, 100.0};
    float   incData[]  = {  1.0,   2.0,  -2.5,  -0.5};

    EGLDisplayState *eglState = eglOpenDisplay(0 /* min win dimension */,
                                               8 /* min color component size */,
                                               8 /* min alpha size */,
                                               0 /* min Z depth */,
                                               EGL_OPENVG_BIT /* render type */);
    path = vgCreatePath(VG_PATH_FORMAT_STANDARD,
                        VG_PATH_DATATYPE_F,
                        1.0, 0.0,
                        0,   0,
                        VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY);
    vgAppendPathData(path, 3, pathSegments, pathData);
    paintStroke = vgCreatePaint();
    vgSetColor(paintStroke, 0x00FF00FF);
    vgSetParameteri(paintStroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetPaint(paintStroke, VG_STROKE_PATH | VG_FILL_PATH);
    vgSetf(VG_STROKE_LINE_WIDTH, 5.0);
    vgLoadIdentity();
    for (rotate = 0.0; rotate < 1000.0; rotate++) {
        color[0] = rotate / 1000.0;
        vgSetfv(VG_CLEAR_COLOR, 4, color);
        vgClear(0, 0, eglState->win.width, eglState->win.height);
        bounce(2, pathData, incData, eglState->win.width, eglState->win.height);
        vgModifyPathCoords(path, 0, 3, pathData);
        vgDrawPath(path, VG_STROKE_PATH);
        eglUpdateDisplay(eglState);
    }
    eglCloseDisplay(eglState);
}