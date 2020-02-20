#pragma once
#include "material1D.h"

namespace MAT_CTL {
	enum state
	{
		TENSION, ELASTIC, CRACK, DESCEND, CRUSH, UNLOAD
	};
}

/**
 * @brief      The trilinear concrete material class.
 */
class concreteTrilinear :
	public material1D
{
public:
	/**
	 * @brief      Constructs a new instance of concreteTrilinear.
	 *
	 * @param[in]  id         The identifier
	 * @param[in]  E          The initial compressive module
	 * @param[in]  fc         The compressive strength
	 * @param[in]  epsilon_c  The corresponding strain of compressive strength
	 * @param[in]  sigma_cr   The compressive crack stress
	 * @param[in]  sigma_u    The compressive crush stress
	 * @param[in]  epsilon_u  The compressive crush strain
	 */
	concreteTrilinear(const int id, const double E, const double fc, const double epsilon_c,
		const double sigma_cr, const double sigma_u, const double epsilon_u);
	~concreteTrilinear();

	/**
	 * @brief      Sets the strain.
	 *
	 * @param[in]  strain  The strain
	 */
	void setStrain(const double *strain) override;

	/**
	 * @brief      Gets the response.
	 *
	 * @param[in]  update  If update the history variables
	 */
	void getResponse(const bool update = false) override;

	material1D *copy() override { return new concreteTrilinear(id, E0, fc, epsilon_c, sigma_cr, sigma_u, epsilon_u); }

	void compressionEnvelope(const double strain, double &stress, double &tangent, MAT_CTL::state &s);

    double fc;				 	///< compressive strength
    double epsilon_c; 			///< the corresponding strain of compressive strength
    double sigma_cr; 			///< compressive crack stress
    double epsilon_cr; 			///< compressive crack strain
    double sigma_u; 			///< compressive crush stress
    double epsilon_u; 			///< compressive crush strain
    double E0;					///< initial module
    double E1;					///< post-crack module
    double E2; 					///< post-peak module
    double epsilon_origin; 		///< strain where the compressive stress begins to be zero when unloading
    double sigma_max; 			///< compressive stress at maximum compressive strain
    double epsilon_max; 		///< maximum compressive strain
    double E_p; 				///< tangent module of previous step
    double sigma_p; 			///< stress of previous step
    double epsilon_p; 			///< strain of previous step
	MAT_CTL::state status; 		///< states of the current step
	MAT_CTL::state status_p; 	///< states of the previous step
};

