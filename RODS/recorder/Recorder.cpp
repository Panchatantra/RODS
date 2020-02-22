#include "Recorder.h"

Recorder::Recorder(const int id, Response rtype, char * fileName) :
Basis(id), n(0), rtype(rtype), fileName(fileName)
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
}

void Recorder::setFileName(char *fileName)
{
	this->fileName = fileName;
}
