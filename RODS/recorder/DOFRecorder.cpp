#include "DOFRecorder.h"

DOFRecorder::DOFRecorder(const int id, std::vector<DOF *> dofs, response rtype, char * fileName):
	Recorder(id, rtype, fileName), dofs(dofs)

{
	n = dofs.size();
}

DOFRecorder::~DOFRecorder()
{
}

void DOFRecorder::record(const int cstep, const double ctime)
{
	Res(cstep, 0) = ctime;
	for (size_t i = 0; i < n; i++)
	{
		DOF * d = dofs[i];
		switch (rtype)
		{
		case DISP:
			Res(cstep, i + 1) = d->dsp;
			break;
		case VEL:
			Res(cstep, i + 1) = d->vel;
			break;
		case ACC:
			Res(cstep, i + 1) = d->acc;
			break;
		case FORCE:
			break;
		case DEF:
			break;
		default:
			break;
		}
	}
}
