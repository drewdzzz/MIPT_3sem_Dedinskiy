#ifndef TEXT_TOOLS_INCLUDE
#define TEXT_TOOLS_INCLUDE
#include<cstdlib>

const int BUF_SIZE = 15;

void draw_text_impl(const char *text, int length, double x, double y, void* font, Color color);

void itoa(int num, char* str);

int strlen(const char* str);


#endif