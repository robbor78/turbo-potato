#include "fire_algo.h"


double fire[ROWS][COLS] = {0};

void init()
{
	fire[0][COLS/2]=1.0;


	for (int row=0; row<ROWS; row++) {

		for (int col=0; col<COLS; col++) {
			std::cout << fire[row][col] << ",";
		}
		std::cout << std::endl;
	}

}

double update(double fire[ROWS][COLS], int row, int col) {

	double value = fire[row][col] * 0.95;

	if (row>0) {		value += fire[row-1][col] * 0.1;	}
	if (col>0) {		value += fire[row][col-1] * 0.1;	}
	if (row<ROWS) {		value += fire[row+1][col] * 0.1;	}
	if (col<COLS) {		value += fire[row][col+1] * 0.1;	}
	if (row>0 && col>0) {		value += fire[row-1][col-1] * 0.1;	}
	if (row>0 && col<COLS) {		value += fire[row-1][col+1] * 0.1;	}
	if (row<ROWS && col<COLS) {		value += fire[row+1][col+1] * 0.1;	}
	if (row<ROWS && col>0) {		value += fire[row+1][col-1] * 0.1;	}
	return std::min(value,1.0);
}

void update()
{
	double temp[ROWS][COLS] = {0};

	for (int row=0; row<ROWS; row++) {
		for (int col=0; col<COLS; col++) {
			temp[row][col] = update(fire,row,col);

			std::cout << temp[row][col] << std::endl;
		}
	}

	std::copy(&temp[0][0], &temp[0][0]+ROWS*COLS,&fire[0][0]);

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

Area getArea(int row, int col) {
	Area a;

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

