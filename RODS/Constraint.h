#pragma once
#include "Basis.h"
#include "DOF.h"
#include <vector>

using namespace std;

/**
 * @brief      The Constraint Class.
 * @details    
 */
class Constraint : public Basis
{
public:

	Constraint();

	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  id    The identifier
	 */
	Constraint(const int id);

	~Constraint();

};