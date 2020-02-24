#pragma once
#include "Recorder.h"
#include "element/Element.h"

/**
 * @brief      The element response recorder.
 */
class ElementRecorder :
	public Recorder
{
public:
	ElementRecorder(const int id, std::vector<Element *> eles, RODS::Response rType, char * fileName);
	~ElementRecorder();

	void record(const int cstep, const double ctime) override;

	std::vector<Element *> eles;
};