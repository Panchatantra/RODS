#pragma once
#include "Recorder.h"
#include "element/Element.h"

#include "json.hpp"
using json = nlohmann::json;

/**
 * @brief      The element response recorder.
 */
class ElementRecorder :
	public Recorder
{
public:
	ElementRecorder();
	ElementRecorder(const int id, std::vector<Element *> eles, RODS::Response rType, const char * fileName);
	ElementRecorder(const int id, RODS::Response rType, const char * fileName);
	~ElementRecorder();

	void record(const int cstep, const double ctime) override;
	void init(const int nsteps);
	void add_ele(Element *ele);
	void remove_ele(Element *ele);
	void get_ele_id(int *id);

	std::vector<Element *> eles;
	std::vector<int> Elements;

	friend void to_json(json& jsonObj, const ElementRecorder& ElementRecorderObj) {
		jsonObj["id"] = ElementRecorderObj.id;
		jsonObj["Elements"] = ElementRecorderObj.Elements;
		jsonObj["rtype"] = ElementRecorderObj.rtype;
		jsonObj["fileName"] = ElementRecorderObj.fileName;
	}
	friend void from_json(const json& jsonObj, ElementRecorder& ElementRecorderObj) {
		jsonObj.at("id").get_to(ElementRecorderObj.id);
		jsonObj.at("Elements").get_to(ElementRecorderObj.Elements);
		jsonObj.at("rtype").get_to(ElementRecorderObj.rtype);
		jsonObj.at("fileName").get_to(ElementRecorderObj.fileName);
	};
};