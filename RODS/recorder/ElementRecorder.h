#pragma once
#include "Recorder.h"
#include "element/Element.h"

class ElementRecorder :
	public Recorder
{
public:
	ElementRecorder(const int id, std::vector<Element *> eles, response rtype, char * fileName);
	~ElementRecorder();

	void record(const int cstep, const double ctime) override;

	std::vector<Element *> eles;
};