#include "recorder.h"

recorder::recorder()
{
}

recorder::~recorder()
{
}

void recorder::init(const int nsteps)
{
	this->nsteps = nsteps;
	Res = zeros<mat>(nsteps, n + 1);
}

void recorder::save()
{
	Res.save(fileName, csv_ascii);
}

void recorder::setFileName(char *fileName)
{
	this->fileName = fileName;
}
