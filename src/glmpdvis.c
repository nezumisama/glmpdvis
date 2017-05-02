/*
    Copyright 2012 by Nezumisama

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <GL/glew.h>
#include <GL/glfw.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <fftw3.h>
#include <stdint.h>

#define PI2 6.283185307179586f
#define SAMPLES 128
#define BLOCKS 128
#define POINTSPERCOL 2

typedef struct { float x, y, z, r, g, b, a, s; } point;
//typedef struct { float x, y, z, s; } point;

int fd = -1;
float rot = 0.0;
float dt;
unsigned pos = 1;
GLuint vbo;
point buff[SAMPLES*BLOCKS*POINTSPERCOL];
int16_t tmp_buff[SAMPLES];
float data[SAMPLES*BLOCKS];
fftw_plan fft_plan;
double fft_in_buff[SAMPLES];
fftw_complex fft_out_buff[SAMPLES/2+1];
point column[] = {
    { 0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f },
    { 0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f }
};

/*
point column[] = {
    { 0.5f, 0.0f, 0.5f, 0.0f },
    { 0.5f, 1.0f, 0.5f, 0.0f }
};
*/
/*
point column[] = {
    // top
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 1.0f, 0.0f }
};
*/
/*
point column[] = {
    // top
    { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }
};
*/
/*
point column[] = {
    // front
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { 1.0f, 0.0f, 1.0f, 0.0f },
    // right
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 1.0f, 0.0f },
    // left
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
    // back
    { 1.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f, 0.0f },
    // top
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 1.0f, 0.0f }
    // bottom
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
};
*/

void cleanup() {
    glDeleteBuffers(1, &vbo);
    close(fd);
    glfwTerminate();
}

void err(const char * s) {
    puts(s);
    exit(EXIT_FAILURE);
}

void PointScaleY(point* p, float scale) {
    p->y *= scale;
}

void PointTranslate(point* p, float x, float y, float z) {
    p->x += x; p->y += y; p->z += z;
}

void GenBuff(void) {
    unsigned ii, i, j, k;
    point* p;

    for (i=0; i<BLOCKS; ++i) {
        ii = (i + pos) % BLOCKS;
        for (j=0; j<SAMPLES; ++j) {
            p = &buff[POINTSPERCOL*(i*SAMPLES+j)];
            memcpy(p, column, sizeof(column));
            for (k=0; k<POINTSPERCOL; ++k) {
                PointScaleY(p+k, 30.0f*data[ii*SAMPLES+j]);
                PointTranslate(p+k, ((float) j) - (SAMPLES/2.0f), 0.0f,
                               ((float) i) - (BLOCKS/2.0f));
                //(p+k)->r = i*1.0f/BLOCKS;
                //(p+k)->g = 1.0f-(p+k)->r;
                (p+k)->r = data[ii*SAMPLES+j];
                (p+k)->g = data[ii*SAMPLES+j]*0.8;
                (p+k)->b = data[ii*SAMPLES+j]*0.5;
            }
        }
    }
}

/*
void GenTestData(void) {
    unsigned i, j;
    float pert;
    float x, y;
    for (i=0; i<BLOCKS; ++i) {
        for (j=0; j<SAMPLES; ++j) {
            //pert = (rand() % 1000)*1.0e-3f;
            //data[i*SAMPLES+j] = pert;
            y = i*4.0f / SAMPLES - 2.0f;
            x = j*4.0f / SAMPLES - 2.0f;
            data[i*SAMPLES+j] = 1.0f*expf(-1.0f*(x*x+y*y));
            //data[i*SAMPLES+j] = pert+5.0f*(1.2+sinf(y*PI2))*
            //                    (1.2+sinf(x*PI2));
        }
    }
}
*/

void gl_init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glShadeModel(GL_SMOOTH);
    glGenBuffersARB(1, &vbo);
    glBindBufferARB(GL_ARRAY_BUFFER, vbo);
    glBufferDataARB(GL_ARRAY_BUFFER, sizeof(buff), 0, GL_DYNAMIC_DRAW);
    glVertexPointer(3, GL_FLOAT, sizeof(point), (void*) 0);
    glColorPointer(4, GL_FLOAT, sizeof(point), (void*) (sizeof(float)*3));
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glClearDepth(1.0);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glColor4f(1.0f, 0.8f, 0.5f, 0.2f);
    /*
    GenTestData();
    dt = glfwGetTime();
    GenBuff();
    dt = glfwGetTime() - dt;
    printf("Buffer generation took %.2f msec.\n", dt*1.0e3f);
    */
    glLineWidth(3.0f);
}

void display(void) {
    glBufferSubDataARB(GL_ARRAY_BUFFER, 0, sizeof(buff), buff);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -170.0f);
    glRotatef(-20.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    //glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    //glScalef(0.1f, 0.1f, 0.1f);
    //glDrawArrays(GL_QUADS, 0, SAMPLES*BLOCKS*POINTSPERCOL);
    glDrawArrays(GL_LINES, 0, SAMPLES*BLOCKS*POINTSPERCOL);
    glfwSwapBuffers();
    //rot += 1.0f;
}

void GLFWCALL reshape(int w, int h) {
    float aspect;

    aspect = (float) w / (float) h;
    glViewport (0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // left right down up far near
    // glOrtho(-aspect*0.6, aspect*0.6, -0.6, 0.6, -1, 1);
    gluPerspective(60.0f, aspect, 100.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    int running = 1, i;
    double scale = 32.0/SAMPLES;
    if (atexit(cleanup)) err("Can't register atexti callback.");
    if (!glfwInit()) err("Can't initalise glfw.");
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 1);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 4);
    if (!glfwOpenWindow(512, 512, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
        err("Can't open a window.");
    glfwSetWindowTitle("OpenGL music visualizer for MPD");
    glfwSwapInterval(1);
    glfwSetWindowSizeCallback(reshape);
    if (glewInit() != GLEW_OK) err("GLEW Initialisation failed.");
    memset(data, 0, sizeof(data));
    //memset(buff, 0, sizeof(buff));
    gl_init();
    fd = open("/tmp/mpd.fifo", O_RDONLY | O_NONBLOCK);
    printf("Calculating optimal fft algo...\n");
    fft_plan = fftw_plan_dft_r2c_1d(SAMPLES, fft_in_buff, fft_out_buff,
                                    FFTW_MEASURE);
    printf("Done.\n");
    while (running) {
        if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
            running = 0;
        // read data from the fifo
        if (read(fd, tmp_buff, SAMPLES*2) < 1) {
            // if no data available, just put zeros
            //memset(&data[pos*SAMPLES], 0, SAMPLES*sizeof(float));
            //memcpy(&data[pos*SAMPLES], &data[((pos-1)%BLOCKS)*SAMPLES],
            //       SAMPLES*sizeof(float));
            //for (i=0; i<SAMPLES; ++i) data[pos*SAMPLES+i] = 1.0f;
            //pos = (pos - 1) % BLOCKS;
            ;
        }
        else {
            // convert data to double
            for (i=0; i<SAMPLES; ++i) fft_in_buff[i] =
                                          tmp_buff[i]*3.0517578125e-05;
            // execute the planned fft
            fftw_execute(fft_plan);
            // put spectrum magnitudes into the data buffer
            for (i=1; i<(SAMPLES/2)+1; ++i) {
                // skip 0-th since it's the DC part of the spectrum
                data[pos*SAMPLES+(i-1)*2] = data[pos*SAMPLES+(i-1)*2+1] =
                    (float) (sqrt(fft_out_buff[i][0]*fft_out_buff[i][0] +
                                  fft_out_buff[i][1]*fft_out_buff[i][1])
                             *scale);
            }
            //for (i=0; i<SAMPLES; ++i) data[pos*SAMPLES+i] =
            //    tmp_buff[i]*3.0517578125e-04;
        }
        pos = (pos + 1) % BLOCKS;
        GenBuff();
        //dt = glfwGetTime();
        display();
        //dt = glfwGetTime() - dt;
        //printf("Frame processing took %.2f msec.\n", dt*1.0e3f);
    }
    return EXIT_SUCCESS;
}
