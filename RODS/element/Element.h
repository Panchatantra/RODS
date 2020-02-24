#pragma once
#include "Basis.h"
#include <armadillo>

using namespace arma;

namespace RODS {
	/// Local axis of rod-like elements.
	enum class LocalAxis
	{
		U1, ///< The axial axis of a rod
		U2, ///< The normal axis of a rod
		U3  ///< The normal axis of a rod
	};
};

/**
 * @brief      The base class of element.
 */
class Element : public Basis
{
public:
	Element();
	Element(const int id);
	~Element();

	virtual void buildMatrix() = 0;
	virtual void getResponse(const bool update = false) = 0;
	virtual void assembleInitialStiffnessMatrix(mat &K0) = 0;
	virtual void assembleStiffnessMatrix(mat &K) = 0;
	virtual void assembleDampingMatrix(mat &C) = 0;
	virtual void assembleMassMatrix(mat &M) = 0;
	virtual void assembleNonlinearForceVector(vec &q) = 0;

	virtual void printResponse();

	int nv;
	double *force, *deformation;
	static double dt;
	static bool isStatic;
};