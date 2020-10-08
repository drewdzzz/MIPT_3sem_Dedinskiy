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

void draw_text_and_measures(Color text_color, Color measure_color) {
    char buffer[BUF_SIZE];

        colors::QUICKSORT.set();
		// draw_text_impl(AnConst::QUICKSORT_BUTTON_TEXT, sizeof(AnConst::QUICKSORT_BUTTON_TEXT), 100, 100, GLUT_BITMAP_TIMES_ROMAN_24);
		colors::BUBBLESORT.set();
        // draw_text_impl(AnConst::BUBBLESORT_BUTTON_TEXT, sizeof(AnConst::BUBBLESORT_BUTTON_TEXT), 100, 80, GLUT_BITMAP_TIMES_ROMAN_24);
		
        text_color.set();
        // draw_text_impl(AnConst::F_BUTTON_TEXT, sizeof(AnConst::F_BUTTON_TEXT), 100, 60, GLUT_BITMAP_TIMES_ROMAN_24);
		// draw_text_impl(AnConst::EXIT_DECLARATION, sizeof(AnConst::EXIT_DECLARATION), 100, 40, GLUT_BITMAP_TIMES_ROMAN_24);
        
        
        measure_color.set();
        // draw_text_impl(AnConst::ARRAY_SIZE, sizeof(AnConst::ARRAY_SIZE), 150, 153, GLUT_BITMAP_HELVETICA_12);
        // draw_text_impl(AnConst::ARRAY_SIZE, sizeof(AnConst::ARRAY_SIZE), 350, 153, GLUT_BITMAP_HELVETICA_12);
        // draw_text_impl(AnConst::COPY_NUM, sizeof(AnConst::COPY_NUM), 14, 383, GLUT_BITMAP_HELVETICA_12);
        // draw_text_impl(AnConst::COMP_NUM, sizeof(AnConst::COMP_NUM), 214, 383, GLUT_BITMAP_HELVETICA_12);
        itoa(AnConst::MAX_COPY_QUANTITY, buffer);
        // draw_text_impl(buffer, strlen(buffer), 23, 373, GLUT_BITMAP_HELVETICA_12);
        itoa(AnConst::MAX_COMP_QUANTITY, buffer);
        // draw_text_impl(buffer, strlen(buffer), 223, 373, GLUT_BITMAP_HELVETICA_12);
        itoa(AnConst::MEASURE_NUM * AnConst::MEASURE_FREQ, buffer);
        // draw_text_impl(buffer, strlen(buffer), 174, 165, GLUT_BITMAP_HELVETICA_12);
        // draw_text_impl(buffer, strlen(buffer), 374, 165, GLUT_BITMAP_HELVETICA_12);   
}


#endif