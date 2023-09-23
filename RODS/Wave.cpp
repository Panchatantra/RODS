#include "Wave.h"

Wave::Wave(const int id, const double dt, const vec &s):
	Basis(id), nsteps((int)s.n_rows), dt(dt), series(vec(s))
{
	name = std::to_string(id);
}

Wave::Wave(const int id, const double dt, char* filePathName):
	Basis(id), dt(dt)
{
	std::string file_path_name = filePathName;
	name = file_path_name.substr(file_path_name.find_last_of("/\\") + 1);

	series.load(filePathName, raw_ascii);
	nsteps = (int)series.n_rows;
}

Wave::Wave(const int id, const double dt, std::string file_path_name):
	Basis(id), dt(dt)
{
	name = file_path_name.substr(file_path_name.find_last_of("/\\") + 1);

	series.load(file_path_name, raw_ascii);
	nsteps = (int)series.n_rows;
}

Wave::Wave(const int id, const double dt) :
	Basis(id), dt(dt), nsteps(0)
{
	name = std::to_string(id);
}

Wave::~Wave()
{
}

void Wave::load(char *filePathName)
{
	series.load(filePathName, raw_ascii);
	nsteps = (int)series.n_rows;
}
