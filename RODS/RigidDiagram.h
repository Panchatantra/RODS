#pragma once

#include "Constraint.h"
#include "Node.h"
#include <vector>
#include <map>

#include "json.hpp"
using json = nlohmann::json;

using namespace std;

#include <armadillo>
using namespace arma;

/**
 * @brief      The Degree-Of-Freedom.
 * @details    DOF is the basic unit for analysis in RODS.
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
	 * @brief      Adds a Node.
	 *
	 * @param      node  The Node
	 */
	void setMasterNode(Node *node);

	/**
	 * @brief      Adds a Node.
	 *
	 * @param      node  The Node
	 */
	void addSlaveNode(Node *node);

	void addSlaveNode(vector<Node *> nodes);

	void assembleConstraintMatrix(mat &A);

	int masterNodeId;
	Node *masterNode;
	int numSlaveNodes;
	map<int, Node *> slaveNodes;

	mat::fixed<3, 6> A_i;

};