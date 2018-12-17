#include <stdio.h>

void initFifo();
int getFifo();
void tileFifo(float, float, char);
void draw_flagFifo();
void setup_requestFifo();
void drawTextFifo(char *, float, float, float);
void setup_pagesFifo();
void displayFifo();
void idleFifo();
void mouseFifo(int ,int ,int ,int);
void handle_bg_colorFifo(int);
void handle_tile_colorFifo(int);
void menuFifo(int);
void addMenuFifo();
void displayPageFifo();