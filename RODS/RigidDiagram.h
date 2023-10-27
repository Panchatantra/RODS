#pragma once

#include "Constraint.h"
#include "Node.h"
#include <vector>
#include <map>

using namespace std;

#include <armadillo>
using namespace arma;

/**
 * @brief      The RigidDiagram Class.
 * @details    
 */
class RigidDiagram : public Constraint
{
public:

	RigidDiagram();

	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  master_node   The master_node
	 */
	RigidDiagram(const int id, Node *master_node);
	RigidDiagram(const int id, Node *master_node, vector<Node *> nodes);

	~RigidDiagram();

	/**
	 * @brief      Set the master Node.
	 *
	 * @param      node  The ptr to the master Node.
	 */
	void setMasterNode(Node *node);

	/**
	 * @brief      Add a slave Node.
	 *
	 * @param      node  The ptr to the slave Node.
	 */
	void addSlaveNode(Node *node);

	void addSlaveNode(vector<Node *> nodes);

	void removeSlaveNode(const int id);

	void setSlaveNodeConsEqn(int &eqn);

	void assembleConstraintMatrix(mat &A);

	int masterNodeId;
	Node *masterNode;
	int numSlaveNodes;
	map<int, Node *> slaveNodes;
	vector<int> slaveNodeIds;

	mat::fixed<3, 6> A_i;

	friend void to_json(json& jsonObj, const RigidDiagram& RigidDiagramObj) {
		jsonObj["id"] = RigidDiagramObj.id;
		jsonObj["masterNodeId"] = RigidDiagramObj.masterNodeId;
		jsonObj["slaveNodeIds"] = RigidDiagramObj.slaveNodeIds;
	}

	friend void from_json(const json& jsonObj, RigidDiagram& RigidDiagramObj) {
		jsonObj.at("id").get_to(RigidDiagramObj.id);
		jsonObj.at("masterNodeId").get_to(RigidDiagramObj.masterNodeId);
		jsonObj.at("slaveNodeIds").get_to(RigidDiagramObj.slaveNodeIds);
	};
};