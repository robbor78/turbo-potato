#include "fire_algo.h"

#include <random>

double fire[ROWS][COLS] = {0};

std::default_random_engine generator;
std::uniform_int_distribution<int> distCol(1,COLS);
std::uniform_int_distribution<int> distValue(1,100);

void init()
{
	fire[0][COLS/2]=1.0;


	for (int row=0; row<ROWS; row++) {

		for (int col=0; col<COLS; col++) {
			//			std::cout << fire[row][col] << ",";
		}
		//	std::cout << std::endl;
	}

}

double transfer(double fire[ROWS][COLS], int row, int col, double gain) 
{
	if (row>=0 && row<ROWS && col>=0 && col<COLS) 
	{
		double div=5.0;
		if (col==0 || col==COLS-1) 
		{
			div=3.0;
		}
		return gain * fire[row][col] / div;
	}
	return 0.0;
}

double update(double fire[ROWS][COLS], int row, int col) {

	double gain=0.4;
	double loss=0.005;

	double value = fire[row][col] * (1.0-gain-loss);

	value+=transfer(fire, row-1,col, gain);
	value+=transfer(fire, row-1,col-1, gain);
	value+=transfer(fire, row-1,col+1, gain);
	value+=transfer(fire, row,col-1, gain);
	value+=transfer(fire, row,col+1, gain);

	/*	if (row>0) {		value += fire[row-1][col] * 0.25;	}
				if (col>0) {		value += fire[row][col-1] * 0.20;	}
	//if (row<ROWS) {		value += fire[row+1][col] * 0.1;	}
	if (col<COLS) {		value += fire[row][col+1] * 0.20;	}
	if (row>0 && col>0) {		value += fire[row-1][col-1] * 0.25;	}
	if (row>0 && col<COLS) {		value += fire[row-1][col+1] * 0.25;	}
	//if (row<ROWS && col<COLS) {		value += fire[row+1][col+1] * 0.1;	}
	//if (row<ROWS && col>0) {		value += fire[row+1][col-1] * 0.1;	}
		*/
	std::cout << value << std::endl;

	return std::min(value,1.0);
}

void update()
{
	double temp[ROWS][COLS] = {0};

	for (int row=0; row<ROWS; row++) {
		for (int col=0; col<COLS; col++) {
			temp[row][col] = update(fire,row,col);

			//			std::cout << temp[row][col] << ", ";		
		}
		//	std::cout << std::endl;

	}

	std::copy(&temp[0][0], &temp[0][0]+ROWS*COLS,&fire[0][0]);

	if ((int)distValue(generator) > 35) 
	{
		//	fire[0][(int)(COLS/2.0)]=1.0;
		fire[0][distCol(generator)%COLS]=1.0;//((double)distValue(generator))/100.0;
	}
}

int fromHsv(double hue, double sat, double val)
{
	double g, b;
	double r = g = b = 0;

	if (sat == 0)
	{
		// Gray scale
		r = g = b = val;
	}
	else
	{
		if (hue==1.0)
		{
			hue = 0;
		}

		hue *= 6.0;
		int i = (int)floor(hue);
		double f = hue - i;
		double aa = val * (1 - sat);
		double bb = val * (1 - (sat * f));
		double cc = val * (1 - (sat * (1 - f)));
		switch (i)
		{
			case 0:
				r = val;
				g = cc;
				b = aa;
				break;
			case 1:
				r = bb;
				g = val;
				b = aa;
				break;
			case 2:
				r = aa;
				g = val;
				b = cc;
				break;
			case 3:
				r = aa;
				g = bb;
				b = val;
				break;
			case 4:
				r = cc;
				g = aa;
				b = val;
				break;
			case 5:
				r = val;
				g = aa;
				b = bb;
				break;
		}
	}

	return (((int)(r * 255))<<16) + (((int)(g * 255))<<8) + (int)(b * 255);
}

int getColor(double value) {
	double color = std::max(value,0.99);
	return fromHsv(color,color,color);
}

#define MAX_COLOURS 5
double colourMap[MAX_COLOURS][4] = {
	{0.0,  0.0,0.0,0.0},
	{0.04,  1.0,0.2,0.2},
	{0.1,  1.0,0.7,0.2},
	{0.9,  1.0,0.9,0.3},
	{1.0,  1.0,1.0,1.0}
};

int getColor2(double value) 
{
	// http://stackoverflow.com/questions/2245842/sorting-colors-in-matlab</url>

	double low=colourMap[0][0], high=colourMap[1][0];

	double redMin=colourMap[0][1], redMax=colourMap[1][1], greenMin=colourMap[0][2], greenMax=colourMap[1][2], blueMin=colourMap[0][3], blueMax=colourMap[1][3];


	int i = 0;
	while (value > high && i<MAX_COLOURS) {
		i++;
		low=high;
		redMin = redMax;
		greenMin = greenMax;
		blueMin = blueMax;
		high = colourMap[i][0];
		redMax = colourMap[i][1];
		greenMax = colourMap[i][2];
		blueMax = colourMap[i][3];
	}

	double r = (value-low)/(high-low);

	double red = redMin + r * (redMax - redMin);
	double green = greenMin + r * (greenMax - greenMin);
	double blue = blueMin + r * (blueMax - blueMin);

	return (((int)(red * 255))<<16) + (((int)(green * 255))<<8) + (int)(blue * 255);
}

Area getArea(int _row, int col) {
	Area a;

	int row = ROWS-_row-1;

	{
		double ratio = (double)WIDTH/(double)COLS;
		//std::cout << "ratio: " << ratio << std::endl;
		a.xmin = (int)(col*ratio);
		a.xmax = a.xmin + (int)(ratio-1.0);
	}

	{
		double ratio = (double)HEIGHT/(double)ROWS;
		//std::cout << "ratio: " << ratio << std::endl;
		a.ymin = (int)(row*ratio);
		a.ymax = a.ymin + (int)(ratio-1.0);
	}

	//	std::cout << " x: " << a.xmin << "-" << a.xmax;
	//	std::cout << " y: " << a.ymin << "-" << a.ymax << std::endl;
	return a;

}

