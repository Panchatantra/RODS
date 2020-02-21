#pragma once
#include "Recorder.h"
#include "element/Element.h"

class elementRecorder :
	public Recorder
{
public:
	elementRecorder(const int id, std::vector<Element *> eles, response rtype, char * fileName);
	~elementRecorder();

	virtual void record(const int cstep, const double ctime);

	std::vector<Element *> eles;
};