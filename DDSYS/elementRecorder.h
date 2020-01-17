#pragma once
#include "recorder.h"
#include "element.h"

class elementRecorder :
	public recorder
{
public:
	elementRecorder(const int id, std::vector<element *> eles, response rtype, char * fileName);
	~elementRecorder();

	void record(const int cstep, const double ctime);

	std::vector<element *> eles;
};

