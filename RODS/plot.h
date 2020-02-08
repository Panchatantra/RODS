#pragma once

#include <stdio.h>
#include <stdlib.h>

static void plot(char *fileName, int col)
{
	FILE* gp = _popen("gnuplot.exe", "w");
	fprintf(gp, "set term pdf font \"Times-New-Roman, 12\"\n");
	char str[80];
	sprintf_s(str, "set output \"%s.pdf\"\n", fileName);
	fprintf(gp, str);
	sprintf_s(str, "plot \"%s\" using 1:%d w l\n", fileName, col);
	fprintf(gp, str);
	//fprintf(gp, "pause mouse\n");
	_pclose(gp);
}