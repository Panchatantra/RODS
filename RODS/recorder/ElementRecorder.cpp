#include "ElementRecorder.h"

ElementRecorder::ElementRecorder() :
	Recorder()
{
}

ElementRecorder::ElementRecorder(const int id, std::vector<Element *> eles, RODS::Response rType, const char * fileName):
	Recorder(id, rType, fileName), eles(eles)
{
	n = 0;
	for (auto it = eles.begin(); it != eles.end(); it++)
	{
		n += (*it)->nv;
	}
	if (rType == RODS::Response::ALL) n *= 2;
}

ElementRecorder::ElementRecorder(const int id, RODS::Response rType, const char *fileName) :
	Recorder(id, rType, fileName)
{
}

ElementRecorder::~ElementRecorder()
{
}

void ElementRecorder::record(const int cstep, const double ctime)
{
	Res(cstep, 0) = ctime;
	int k = 1;

	for (size_t i = 0; i < eles.size(); i++)
	{
		Element *ele = eles[i];
		switch (rtype)
		{
		case RODS::Response::FORCE:
			for (int j = 0; j < ele->nv; j++)
			{
				Res(cstep, k) = ele->force[j];
				k++;
			}
			break;
		case RODS::Response::DEF:
			for (int j = 0; j < ele->nv; j++)
			{
				Res(cstep, k) = ele->deformation[j];
				k++;
			}
			break;
		case RODS::Response::ALL:
			for (int j = 0; j < ele->nv; j++)
			{
				Res(cstep, k) = ele->deformation[j];
				k++;
			}
			for (int j = 0; j < ele->nv; j++)
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
	auto it = std::find(eles.begin(), eles.end(), ele);
	if (it == eles.end())
	{
		eles.push_back(ele);
		Elements.push_back(ele->id);
	}
}

void ElementRecorder::remove_ele(Element *ele)
{
	auto it = std::find(eles.begin(), eles.end(), ele);
	if (it != eles.end())
	{
		eles.erase(it);
		auto IT = Elements.begin() + (it - eles.begin());
		Elements.erase(IT);
	}
}

void ElementRecorder::get_ele_id(int *id)
{
	int i = 0;
	for (auto &it : eles)
	{
		id[i++] = it->id;
	}
}
