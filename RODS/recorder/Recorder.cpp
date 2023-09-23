#include "Recorder.h"

Recorder::Recorder(const int id, RODS::Response rType, char * fileName) :
Basis(id), n(0), rtype(rType), fileName(fileName)
{
}

Recorder::~Recorder()
{
}

void Recorder::init(const int nsteps)
{
	this->nsteps = nsteps;
	Res = zeros<mat>(nsteps, n + 1);
}

void Recorder::save()
{
	Res.save(fileName, raw_ascii);
	Res.clear();
}

void Recorder::setFileName(char *fileName)
{
	this->fileName = fileName;
}
