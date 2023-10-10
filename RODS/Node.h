#pragma once

#include "Basis.h"
#include "DOF.h"

namespace RODS {
	enum class Dimension
	{
		ONE = 1,
		TWO,
		THREE,
		TWO_WITHOUT_ROTATE = 20,
		THREE_WITHOUT_ROTATE = 30,
		TWO_PLATE = 21,
	};
}

/**
 * @brief      The node.
 */
class Node : public Basis
{
public:
	Node();
	Node(const int id, const double x=0.0, const double y=0.0, const double z=0.0);
	~Node();

	void setDof(DOF *d);
	void fixDOF();
	void fixDOF(RODS::Direction dir);
	void freeDOF();
	void freeDOF(RODS::Direction dir);
	bool isActivated(RODS::Direction dir);
	void Deactivate(RODS::Direction dir);

	void setDim(RODS::Dimension dim);
	void setMass(const double m);
	void setMass(const double m, RODS::Direction dir);
	void setMass(const double m, const double I);

	int getIdDof(RODS::Direction dir);
	void getIdsDof(int *ids);

	double x, y, z;
	double x0, y0, z0;

	DOF *dofX, *dofY, *dofZ;
	DOF *dofRX, *dofRY, *dofRZ;

	int IdDofX, IdDofY, IdDofZ;
	int IdDofRX, IdDofRY, IdDofRZ;

	RODS::Dimension dim;

	friend void to_json(json& jsonObj, const Node& NodeObj) {
		jsonObj["id"] = NodeObj.id;
		jsonObj["IdDofX"] = NodeObj.dofX != nullptr ? NodeObj.dofX->id : -1;
		jsonObj["IdDofY"] = NodeObj.dofY != nullptr ? NodeObj.dofY->id : -1;
		jsonObj["IdDofZ"] = NodeObj.dofZ != nullptr ? NodeObj.dofZ->id : -1;
		jsonObj["IdDofRX"] = NodeObj.dofRX != nullptr ? NodeObj.dofRX->id : -1;
		jsonObj["IdDofRY"] = NodeObj.dofRY != nullptr ? NodeObj.dofRY->id : -1;
		jsonObj["IdDofRZ"] = NodeObj.dofRZ != nullptr ? NodeObj.dofRZ->id : -1;
		jsonObj["dim"] = NodeObj.dim;
		jsonObj["x"] = NodeObj.x;
		jsonObj["y"] = NodeObj.y;
		jsonObj["z"] = NodeObj.z;
	}
	friend void from_json(const json& jsonObj, Node& NodeObj) {
		jsonObj.at("id").get_to(NodeObj.id);
		jsonObj.at("IdDofX").get_to(NodeObj.IdDofX);
		jsonObj.at("IdDofY").get_to(NodeObj.IdDofY);
		jsonObj.at("IdDofZ").get_to(NodeObj.IdDofZ);
		jsonObj.at("IdDofRX").get_to(NodeObj.IdDofRX);
		jsonObj.at("IdDofRY").get_to(NodeObj.IdDofRY);
		jsonObj.at("IdDofRZ").get_to(NodeObj.IdDofRZ);
		jsonObj.at("dim").get_to(NodeObj.dim);
		jsonObj.at("x").get_to(NodeObj.x);
		jsonObj.at("y").get_to(NodeObj.y);
		jsonObj.at("z").get_to(NodeObj.z);
	};
};

