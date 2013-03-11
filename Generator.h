#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <cstdlib>
#include "stdafx.h"

class Generator
{
public :
	Generator(int x, int y);
	~Generator();

	int **m;
	int maxx;
	int maxy;

	void print_mas(void);
	void call(int x, int y, int ox, int oy);
	void generate(int x, int y, int ox, int oy);
	void print_mas2(void);
	void mazeGen(void);
};