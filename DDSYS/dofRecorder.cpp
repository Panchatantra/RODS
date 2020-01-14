#include "dofRecorder.h"

dofRecorder::dofRecorder(dof * dofs, const int n, char * fileName, const int nsteps)
{
	this->dofs = dofs;
	this->n = n;

	this->fileName = fileName;
	this->nsteps = nsteps;

	Res = zeros<mat>(nsteps, n+1);
}

dofRecorder::~dofRecorder()
{
}

void dofRecorder::record(const int cstep, const double ctime)
{
	Res(cstep, 0) = ctime;
	for (dof * d = dofs; d < dofs+n; d++)
	{
		Res(cstep, (int)(d-dofs)) = d->dsp;
	}
}

void dofRecorder::save()
{
	Res.save(fileName, csv_ascii);
}
