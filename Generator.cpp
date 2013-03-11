#include "stdafx.h"
#include "Generator.h"

Generator::Generator(int x, int y)
{
	maxx = x;
	maxy = y;

	m = (int **)malloc(maxy * sizeof(int*));
	for (int i = 0; i < maxy; i += 1)
	{
		m[i] = (int *)malloc(maxx * sizeof(int));
		for (int j = 0; j < maxx; j += 1)
		{
			m[i][j] = 1;
		}
	}
}

Generator::~Generator()
{
}

void Generator::print_mas()
{
	for (int j = 0; j < maxy; j += 1)
	{
		for (int i = 0; i < maxx; i += 1)
		{
			if (m[j][i] == 1) 
				printf("%c", 127);
			else if (m[j][i] == 0)
					printf("  ");
			else if (m[j][i] == 2)
					printf("*");
		}
		printf("\n");
	}
	printf("\n");
}

void Generator::generate(int x, int y, int ox, int oy)
{
	if(x + 1 < maxx && y + 1 < maxy && x > 0 && y  > 0){ 	
		if (m[y - 1][x] != 0 || (x == ox && y - 1 == oy)){	
			if(m[y][x - 1] != 0 || (x - 1 == ox && y == oy)){
				if(m[y][x + 1] != 0 || (x + 1 == ox && y == oy)){
					if(m[y + 1][x] != 0 || (x == ox && y + 1 == oy)){
						m[y][x] = 0;
						call(x, y, ox, oy);
					}
				}
			}
		}
	}
}

void Generator::call(int x, int y, int ox, int oy)
{
	srand(time(NULL));
	int i = rand() % 10;
	if(i == 0){
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
	}else if(i == 1){
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
	}else if(i == 2){
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);	
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
	}else if(i == 3){
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);		
	}else if(i == 4){
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
	}else if(i == 5){
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);
	}else if(i == 6){
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);
	}else if(i == 7){
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);	
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);
	}else if(i == 8){
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
	}else if(i == 9){
		if(x != ox || y + 1 != oy) generate(x, y + 1, x, y);
		if(x - 1 != ox || y != oy) generate(x - 1, y, x, y);
		if(x != ox || y - 1 != oy) generate(x, y - 1, x, y);
		if(x + 1 != ox || y != oy) generate(x + 1, y, x, y);
	}		
}

void Generator::print_mas2()
{
	FILE * f = fopen("maze.txt", "w");
	if (f == NULL) return;
	for (int j = 0; j < maxy; j += 1)
	{
		for (int i = 0; i < maxx; i += 1)
		{
			if (m[j][i] == 1) fputc('1', f);
			else if (m[j][i] == 0) fputc('0', f);
		}
		fputc('\n', f);
	}
	fclose(f);
}

void Generator::mazeGen()
{
	srand(time(NULL));
	int rx, ry;
	rx = rand() % maxx;
	srand(time(NULL));
	ry = rand() % maxy;
	generate(rx, ry, maxx, maxy);
	print_mas2();
}