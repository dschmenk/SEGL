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
void cubicloop(float *ctrlPts, float *pathPts)
{
    // MOVE_TO
    pathPts[0] = (ctrlPts[0] + ctrlPts[2]) / 2.0;
    pathPts[1] = (ctrlPts[1] + ctrlPts[3]) / 2.0;
    // CUBIC_TO
    pathPts[2] = ctrlPts[0];
    pathPts[3] = ctrlPts[1];
    pathPts[4] = ctrlPts[4];
    pathPts[5] = ctrlPts[5];
    pathPts[6] = (ctrlPts[4] + ctrlPts[6]) / 2.0;
    pathPts[7] = (ctrlPts[5] + ctrlPts[7]) / 2.0;
    // SCUBIC_TO Connect end back around to start of path
    pathPts[8]  = ctrlPts[2];
    pathPts[9]  = ctrlPts[3];
    pathPts[10] = pathPts[0];
    pathPts[11] = pathPts[1];
}
int main(int argc, char **argv)
{
    int i;
    float color[4] = {0.0, 0.33, 0.33, 1.0};
    VGPath path;
    VGPaint paintStroke;
    VGubyte pathSegments[] = {VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS, VG_SCUBIC_TO_ABS};
    float   pathData[12];
    float   ctrlPts[]  = {100.0, 100.0, 300.0, 300.0,
                           35.0, 400.0, 125.0, 500.0};
    float   incData[]  = {  1.0,   2.0,  -2.5,  -0.5,
                            1.5,   1.0,   1.5,  -0.5};

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
    cubicloop(ctrlPts, pathData);
    vgAppendPathData(path, 3, pathSegments, pathData);
    paintStroke = vgCreatePaint();
    vgSetColor(paintStroke, 0x00FF00FF);
    vgSetParameteri(paintStroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetPaint(paintStroke, VG_STROKE_PATH | VG_FILL_PATH);
    vgSetf(VG_STROKE_LINE_WIDTH, 15.0);
    for (i = 0; i < 1000; i++) {
        color[0] = (float)i / 1000.0;
        vgSetfv(VG_CLEAR_COLOR, 4, color);
        vgClear(0, 0, eglState->win.width, eglState->win.height);
        bounce(4, ctrlPts, incData, eglState->win.width, eglState->win.height);
        cubicloop(ctrlPts, pathData);
        vgModifyPathCoords(path, 0, 3, pathData);
        vgDrawPath(path, VG_STROKE_PATH);
        eglUpdateDisplay(eglState);
    }
    eglCloseDisplay(eglState);
}