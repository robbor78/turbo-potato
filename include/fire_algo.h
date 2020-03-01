#ifndef FIRE_ALGO_H
#define FIRE_ALGO_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "constants.h"

extern double fire[ROWS][COLS];


struct Area{
	int xmin;
	int xmax;
	int ymin;
	int ymax;
};

void init();
double update(double fire[ROWS][COLS], int row, int col);
void update();
int fromHsv(double hue, double sat, double val);
int getColor(double value);
int getColor2(double value);
Area getArea(int row, int col);

#endif
