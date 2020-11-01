#ifndef TEXT_TOOLS_INCLUDE
#define TEXT_TOOLS_INCLUDE
#include<stdlib.h>

const int BUF_SIZE = 15;

namespace {
    void draw_text_impl(const char *text, int length, double x, double y, void* font, Color color) {
        color.set();
        x = 200*x + 200;
        y = 200*y + 200;
        glMatrixMode(GL_PROJECTION);
        double *matrix = new double[16];
        glGetDoublev(GL_PROJECTION_MATRIX, matrix);
        glLoadIdentity();
        glOrtho(0, 400, 0, 400, -5, 5);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glLoadIdentity();
        glRasterPos2d(x, y);
        
        for(int i = 0; i < length; i++) {
            glutBitmapCharacter(font,(int)text[i]);
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(matrix);
        glMatrixMode(GL_MODELVIEW);

        glFlush();
    }
}

void itoa(int num, char* str) {
    int tmp = num;
    int size = 0;
    while (tmp) {
        ++size;
        tmp /= 10;
    }

    str[size] = 0;
    for (int i = size-1; i >= 0; --i){
        str[i] = num % 10 + '0';
        num /= 10;
    }

}

int strlen(const char* str) {
    int size = 0;
    for (size = 0; str[size]; ++size) 
        ;
    return size;
}


#endif