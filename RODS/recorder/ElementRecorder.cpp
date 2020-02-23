#include "ElementRecorder.h"

ElementRecorder::ElementRecorder(const int id, std::vector<Element *> eles, Response rtype, char * fileName):
	Recorder(id, rtype, fileName), eles(eles)
{
	if (rtype == Response::ALL)
	{
		n = 2*eles.size()*eles[0]->nv;
	}
	else
	{
		n = eles.size()*eles[0]->nv;
	}
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
		case Response::FORCE:
			for (int j = 0; j < nv; j++)
			{
				Res(cstep, k) = ele->force[j];
				k++;
			}
			break;
		case Response::DEF:
			for (int j = 0; j < nv; j++)
			{
				Res(cstep, k) = ele->deformation[j];
				k++;
			}
			break;
		case Response::ALL:
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