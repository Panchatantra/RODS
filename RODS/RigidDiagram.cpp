#include "RigidDiagram.h"
#include <iostream>

RigidDiagram::RigidDiagram() :
	Constraint()
{

}

RigidDiagram::RigidDiagram(const int id, Node *master_node) :
	Constraint(id)
{
	setMasterNode(master_node);
}

RigidDiagram::RigidDiagram(const int id, Node *master_node,
							vector<Node *> nodes) :
	Constraint(id)
{
	setMasterNode(master_node);
	addSlaveNode(nodes);
}

RigidDiagram::~RigidDiagram()
{
}

void RigidDiagram::setMasterNode(Node *node)
{
	masterNodeId = node->id;
	masterNode = node;
}

void RigidDiagram::addSlaveNode(Node *node)
{
	if (slaveNodes.count(node->id) == 0)
	{
		slaveNodes[node->id] = node;
		slaveNodeIds.push_back(node->id);
	}
	numSlaveNodes = slaveNodes.size();
}

void RigidDiagram::addSlaveNode(vector<Node *> nodes)
{
	for (auto &node : nodes)
	{
		addSlaveNode(node);
	}
}

void RigidDiagram::removeSlaveNode(const int id)
{
	slaveNodes.erase(id);
	auto it = std::find(slaveNodeIds.begin(), slaveNodeIds.end(), id);
	if (it != slaveNodeIds.end())
		slaveNodeIds.erase(it);
	numSlaveNodes = slaveNodes.size();
}

void RigidDiagram::setSlaveNodeConsEqn(int &eqn)
{
	for (auto &node_pair : slaveNodes)
	{
		node_pair.second->dofX->consEqnId = eqn++;
		node_pair.second->dofY->consEqnId = eqn++;
		node_pair.second->dofRZ->consEqnId = eqn++;
	}
}

void RigidDiagram::assembleConstraintMatrix(mat &A)
{
	A_i = zeros<mat>(3, 6);
	A_i(0, 0) = 1.0;
	A_i(1, 1) = 1.0;
	A_i(2, 2) = 1.0;
	A_i(0, 3) = -1.0;
	A_i(1, 4) = -1.0;
	A_i(2, 5) = -1.0;

	double dx = 0.0, dy = 0.0;
	int local[3] = {0,1,2};

	int master_global[3] = {0,0,0};
	master_global[0] = masterNode->dofX->eqnId;
	master_global[1] = masterNode->dofY->eqnId;
	master_global[2] = masterNode->dofRZ->eqnId;

	int slave_global[3] = {0,0,0};
	int slave_cons_global[3] = {0,0,0};

	if (numSlaveNodes > 0)
	{
		for (auto &node_pair: slaveNodes)
		{
			dx = node_pair.second->x - masterNode->x;
			dy = node_pair.second->y - masterNode->y;
			A_i(0, 2) = -dy;
			A_i(1, 2) = dx;

			slave_global[0] = node_pair.second->dofX->eqnId;
			slave_global[1] = node_pair.second->dofY->eqnId;
			slave_global[2] = node_pair.second->dofRZ->eqnId;
			slave_cons_global[0] = node_pair.second->dofX->consEqnId;
			slave_cons_global[1] = node_pair.second->dofY->consEqnId;
			slave_cons_global[2] = node_pair.second->dofRZ->consEqnId;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
					{
						A(slave_cons_global[i], master_global[j]) = A_i(local[i], local[j]);
						A(slave_cons_global[i], slave_global[j]) = A_i(local[i], local[j]+3);
					}
			}
		}
	}
}
