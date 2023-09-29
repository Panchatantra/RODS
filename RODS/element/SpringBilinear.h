#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The bilinear spring in X direction.
 */
class SpringBilinear : public ROD1D
{
public:
	SpringBilinear();
	SpringBilinear(const int id, DOF *i, DOF *j, const double k0, const double uy, const double alpha);
	~SpringBilinear();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);
	void assembleNonlinearForceVector(vec &q);

	double k0, uy, k1, fy, alpha;
	double k, u, f;
	double kp, up, dup, fp;
	size_t status, sp;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;

	friend void to_json(json& jsonObj, const SpringBilinear& SpringBilinearObj) {
		jsonObj["id"] = SpringBilinearObj.id;
		jsonObj["IdDofI"] = SpringBilinearObj.dofI->id;
		jsonObj["IdDofJ"] = SpringBilinearObj.dofJ->id;
		jsonObj["k0"] = SpringBilinearObj.k0;
		jsonObj["uy"] = SpringBilinearObj.uy;
		jsonObj["alpha"] = SpringBilinearObj.alpha;
	}
	friend void from_json(const json& jsonObj, SpringBilinear& SpringBilinearObj) {
		jsonObj.at("id").get_to(SpringBilinearObj.id);
		jsonObj.at("IdDofI").get_to(SpringBilinearObj.IdDofI);
		jsonObj.at("IdDofJ").get_to(SpringBilinearObj.IdDofJ);
		jsonObj.at("k0").get_to(SpringBilinearObj.k0);
		jsonObj.at("uy").get_to(SpringBilinearObj.uy);
		jsonObj.at("alpha").get_to(SpringBilinearObj.alpha);
	};
};

