#include <math.h>
#include <GLES/gl.h>
#include "segl.h"

void sglViewFrustrum(GLfloat fov, GLfloat near, GLfloat far, GLsizei width, GLsizei height)
{
    float hht;
    float hwd;
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    hht = near * (float)tan(fov / 2.0 / 180.0 * M_PI);
    hwd = hht * (float)width / (float)height;
    glFrustumf(-hwd, hwd, -hht, hht, near, far);
    glMatrixMode(GL_MODELVIEW);
}
void sglColorf(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    static GLfloat color[4*4] = {
        1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0};
    color[0] = color[4] = color[8]  = color[12] = red;
    color[1] = color[5] = color[9]  = color[13] = green;
    color[2] = color[6] = color[10] = color[14] = blue;
    color[3] = color[7] = color[11] = color[15] = alpha;
    glColorPointer(4, GL_FLOAT, 0, color);
}
void sglTrif(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3)
{
    static GLfloat vertex[3*3] = {
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0};
    vertex[0]  = x1;
    vertex[1]  = y1;
    vertex[3]  = x2;
    vertex[4]  = y2;
    vertex[6]  = x3;
    vertex[7]  = y3;
    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
}
void sglRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    static GLfloat vertex[4*3] = {
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0};
    vertex[0]  = x1;
    vertex[1]  = y1;
    vertex[3]  = x2;
    vertex[4]  = y1;
    vertex[6]  = x1;
    vertex[7]  = y2;
    vertex[9]  = x2;
    vertex[10] = y2;
    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
/*******************************************************************************/
#include <stdio.h>

int main(int argc, char **argv)
{
    float rotate;

    EGLDisplayState *eglState = eglOpenDisplay(0 /* min win dimension */,
                                               8 /* min color component size */,
                                               8 /* min alpha size */,
                                               0 /* min Z depth */,
                                               EGL_OPENGL_ES_BIT /* render type */);
    sglViewFrustrum(45.0, 1.0, 100.0, eglState->win.width, eglState->win.height);
    glClearColor(0.10, 0.33, 0.33, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    for (rotate = 0.0; rotate < 360.0; rotate++) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -100.0);
        glRotatef(rotate, 0.0, 0.0, 1.0);
        sglColorf(1.0, 0.0, 0.0, rotate / 360.0);
        sglRectf(-10.0, -10.0, 10.0, 10.0);
        eglUpdateDisplay(eglState);
    }
    eglCloseDisplay(eglState);
}