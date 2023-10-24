#pragma once
#include "Basis.h"
#include "DOF.h"
#include <vector>

#include "json.hpp"
using json = nlohmann::json;

using namespace std;

/**
 * @brief      The Degree-Of-Freedom.
 * @details    DOF is the basic unit for analysis in RODS.
 */
class Constraint : public Basis
{
public:

	Constraint();

	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  dir   The Direction
	 */
	Constraint(const int id);

	~Constraint();

};