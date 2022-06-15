#include "ElementRecorder.h"

ElementRecorder::ElementRecorder(const int id, std::vector<Element *> eles, RODS::Response rType, char * fileName):
	Recorder(id, rType, fileName), eles(eles)
{
	if (rType == RODS::Response::ALL)
	{
		n = 2*eles.size()*eles[0]->nv;
	}
	else
	{
		n = eles.size()*eles[0]->nv;
	}
}

ElementRecorder::ElementRecorder(const int id, RODS::Response rType, char* fileName): Recorder(id, rType, fileName)
{
}

ElementRecorder::~ElementRecorder()
{
}

void ElementRecorder::record(const int cstep, const double ctime)
{
	Res(cstep, 0) = ctime;
	int k = 1;
	int nv = eles[0]->nv;

	for (size_t i = 0; i < eles.size(); i++)
	{
		Element *ele = eles[i];
		switch (rtype)
		{
		case RODS::Response::FORCE:
			for (int j = 0; j < nv; j++)
			{
				Res(cstep, k) = ele->force[j];
				k++;
			}
			break;
		case RODS::Response::DEF:
			for (int j = 0; j < nv; j++)
			{
				Res(cstep, k) = ele->deformation[j];
				k++;
			}
			break;
		case RODS::Response::ALL:
			for (int j = 0; j < nv; j++)
			{
				Res(cstep, k) = ele->deformation[j];
				k++;
			}
			for (int j = 0; j < nv; j++)
			{
				Res(cstep, k) = ele->force[j];
				k++;
			}
		default:
			break;
		}
	}
}

void ElementRecorder::init(const int nsteps)
{
	if (rtype == RODS::Response::ALL)
	{
		n = 2*eles.size()*eles[0]->nv;
	}
	else
	{
		n = eles.size()*eles[0]->nv;
	}
	Recorder::init(nsteps);
}

void ElementRecorder::add_ele(Element* ele)
{
	eles.push_back(ele);
	n = eles.size();
}
