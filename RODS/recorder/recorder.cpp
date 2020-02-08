#include "recorder.h"

recorder::recorder(const int id, response rtype, char * fileName) :
basis(id), n(0), rtype(rtype), fileName(fileName)
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
	Res.save(fileName, raw_ascii);
}

void recorder::setFileName(char *fileName)
{
	this->fileName = fileName;
}
