#include "elementRecorder.h"



elementRecorder::elementRecorder(const int id, std::vector<element *> eles, response rtype, char * fileName)
{
	this->id = id;
	this->eles = eles;
	this->n = eles.size()*eles[0]->nv;
	this->rtype = rtype;
	this->fileName = fileName;
}


elementRecorder::~elementRecorder()
{
}

void elementRecorder::record(const int cstep, const double ctime)
{
	Res(cstep, 0) = ctime;
	int k = 1;
	int nv = eles[0]->nv;

	for (size_t i = 0; i < eles.size(); i++)
	{
		element *ele = eles[i];
		switch (rtype)
		{
		case FORCE:
			for (int j = 0; j < nv; j++)
			{
				Res(cstep, k) = ele->force[j];
				k++;
			}
			break;
		case DEF:
			for (int j = 0; j < nv; j++)
			{
				Res(cstep, k) = ele->deformation[j];
				k++;
			}
			break;
		default:
			break;
		}
	}
}