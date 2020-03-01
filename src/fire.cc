#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <iostream>
#include <cmath>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "fire_algo.h"
#include "constants.h"

void drawpixel(Display* di, Window wi, GC gc, int x, int y, int color)
{
	XSetForeground(di, gc, color);
	XDrawPoint(di, wi, gc, x, y);
}

void draw(Display* di, Window wi, GC gc)
{

	spdlog::info("Drawing...");
	for (int row=0; row<ROWS; row++) {
		for (int col=0; col<COLS; col++) {

			double value = fire[row][col];
			int color = getColor2(value);

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
	spdlog::info("Draw done");
}

int main() 
{

	spdlog::info("fire...");


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
		}


		update();
		draw(di,wi,gc);

	}
	XFreeGC(di, gc);
	XDestroyWindow(di, wi);
	XCloseDisplay(di);
	return 0;
}
