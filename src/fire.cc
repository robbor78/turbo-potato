#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <iostream>
#include <cmath>

#define WIDTH 640
#define HEIGHT 480
#define ROWS 3
#define COLS 3

double fire[ROWS][COLS] = {0};

struct Area{
	int xmin;
	int xmax;
	int ymin;
	int ymax;
};

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
	return value;
}

void update()
{
	double temp[ROWS][COLS] = {0};

	for (int row=0; row<ROWS; row++) {
		for (int col=0; col<COLS; col++) {
			temp[row][col] = update(fire,row,col);
		}
	}

	std::copy(&temp[0][0], &temp[0][0]+ROWS*COLS,&fire[0][0]);

}

void drawpixel(Display* di, Window wi, GC gc, int x, int y, int color)
{
	XSetForeground(di, gc, color);
	XDrawPoint(di, wi, gc, x, y);
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

void draw(Display* di, Window wi, GC gc)
{

	for (int row=0; row<ROWS; row++) {
		for (int col=0; col<COLS; col++) {

			double value = fire[row][col];
			int color = getColor(value);

			Area area = getArea(row,col);

			std::cout << "r: " << row << " c: " << col << " v: " << value;
			std::cout << ", col: " << color;
			std::cout << " x: " << area.xmin << "-" << area.xmax;
			std::cout << " y: " << area.ymin << "-" << area.ymax << std::endl;

			for (int x=area.xmin; x<=area.xmax; x++)
			{

				for (int y=area.ymin; y<=area.ymax; y++)
				{
					drawpixel(di, wi, gc, x, y, color);		

				}
			}
		}
	}
}

int main() 
{

	init();


	//Open Display
	Display *di = XOpenDisplay(getenv("DISPLAY"));
	if (di == NULL) {
		printf("Couldn't open display.\n");
		return -1;
	}

	//Create Window
	int const x = 0, y = 0, width = WIDTH, height = HEIGHT, border_width = 1;
	int sc    = DefaultScreen(di);
	Window ro = DefaultRootWindow(di);
	Window wi = XCreateSimpleWindow(di, ro, x, y, width, height, border_width, 
			BlackPixel(di, sc), WhitePixel(di, sc));
	XMapWindow(di, wi); //Make window visible
	XStoreName(di, wi, "Window sample"); // Set window title

	//Prepare the window for drawing
	GC gc = XCreateGC(di, ro, 0, NULL);

	//Select what events the window will listen to
	XSelectInput(di, wi, KeyPressMask | ExposureMask);
	XEvent ev;
	int quit = 0;
	while (!quit) {
		int a = XNextEvent(di, &ev);
		if (ev.type == KeyPress)
			quit = 1; // quit if someone presses a key
		if (ev.type == Expose) {
			drawpixel(di, wi, gc, 639, 479, 0x000000); //green
			//update();
			draw(di,wi,gc);
		}



	}
	XFreeGC(di, gc);
	XDestroyWindow(di, wi);
	XCloseDisplay(di);
	return 0;
}
