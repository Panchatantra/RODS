#pragma once
#include "Basis.h"
#include "Load.h"
#include <vector>

using namespace std;

constexpr bool FIXED = true;
enum direction { X,Y,Z,RX,RY,RZ };

class DOF : public Basis
{
public:
	DOF(const int id, direction dir);
	DOF(const int id, direction dir, const double m, const bool fixed = false);
	~DOF();

	void setResponse(const double dsp, const double vel, const double acc);
	void addLoad(Load* load);
	double getLoad(const double time, const bool withConst=true);

    int eqnId;
	double mass;
	direction dir;
	vector<Load *> loads;
	bool isFixed;

	double dsp, vel, acc;

	static double g;
};