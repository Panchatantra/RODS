#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The tuned viscous mass damper in X direction.
 */
class TVMD : public ROD1D
{
public:
	TVMD();
	TVMD(const int id, DOF *i, DOF *j, const double m, const double c, const double k);
	~TVMD();

	void getResponse(const bool update = false);
	void assembleNonlinearForceVector(vec &q);

	void func(const double uc, const double ud, const double vd, double &kud, double &kvd);

	double m, c, k;
	double u, f;
	double ud, vd, udp, vdp;

	vec::fixed<2> q;

	friend void to_json(json& jsonObj, const TVMD& TVMDObj) {
		jsonObj["id"] = TVMDObj.id;
		jsonObj["IdDofI"] = TVMDObj.dofI->id;
		jsonObj["IdDofJ"] = TVMDObj.dofJ->id;
		jsonObj["m"] = TVMDObj.m;
		jsonObj["c"] = TVMDObj.c;
		jsonObj["k"] = TVMDObj.k;
	}
	friend void from_json(const json& jsonObj, TVMD& TVMDObj) {
		jsonObj.at("id").get_to(TVMDObj.id);
		jsonObj.at("IdDofI").get_to(TVMDObj.IdDofI);
		jsonObj.at("IdDofJ").get_to(TVMDObj.IdDofJ);
		jsonObj.at("m").get_to(TVMDObj.m);
		jsonObj.at("c").get_to(TVMDObj.c);
		jsonObj.at("k").get_to(TVMDObj.k);
	};
};