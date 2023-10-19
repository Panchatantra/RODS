#pragma once

#include <stdio.h>
#include <stdlib.h>

static inline void gnuplot(char *fileName, int col)
{
#ifdef _WIN32
    FILE* gp = _popen("gnuplot.exe", "w");
	fprintf(gp, "set term pdf font \"Times-New-Roman, 12\"\n");
	char str[80];
	sprintf_s(str, "set output \"%s.pdf\"\n", fileName);
	fprintf(gp, str);
	sprintf_s(str, "plot \"%s\" using 1:%d w l\n", fileName, col);
	fprintf(gp, str);
	//fprintf(gp, "pause mouse\n");
	_pclose(gp);
#else
    FILE* gp = popen("gnuplot", "w");
    fprintf(gp, "set term pdf font \"Times-New-Roman, 12\"\n");
    char str[80];
    sprintf(str, "set output \"%s.pdf\"\n", fileName);
    fprintf(gp, str);
    sprintf(str, "plot \"%s\" using 1:%d w l\n", fileName, col);
    fprintf(gp, str);
    //fprintf(gp, "pause mouse\n");
    pclose(gp);
#endif
}
