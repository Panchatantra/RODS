#include "dofRecorder.h"

dofRecorder::dofRecorder(const int id, std::vector<dof *> dofs, char * fileName) :
	nsteps(0)
{
	this->id = id;
	this->dofs = dofs;
	this->n = dofs.size();

	this->fileName = fileName;
}

dofRecorder::~dofRecorder()
{
}

void dofRecorder::init(const int nsteps)
{
	this->nsteps = nsteps;
	Res = zeros<mat>(nsteps, n+1);
}

void dofRecorder::record(const int cstep, const double ctime)
{
	Res(cstep, 0) = ctime;
	for (int i = 0; i < n; i++)
	{
		dof * d = dofs[i];
		Res(cstep, i+1) = d->dsp;
	}
}

void dofRecorder::save()
{
	Res.save(fileName, csv_ascii);
}
