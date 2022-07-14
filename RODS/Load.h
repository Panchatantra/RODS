#pragma once
#include "Basis.h"
#include <vector>

using namespace std;
/**
 * @brief      The time-varying load.
 */
class Load :
	public Basis
{
public:
	Load();

	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  id          The Load identifier
	 * @param      t           The loading moments
	 * @param      p           The loading values related to the moments
	 * @param[in]  nP          The number of loading moments
	 * @param[in]  arriveTime  The arrive time of the load, default to 0.0
	 * @param[in]  scale       The scale factor of the load, default to 1.0
	 */
	Load(const int id, double *t, double *p, const int nP, const double arriveTime=0.0, const double scale=1.0);
	Load(const int id, const double arriveTime=0.0, const double scale=1.0);
	~Load();

	double addValue(const double time, const double value);
	double getValue(const double time);
	void setConst(const bool keepConst=true);
	void setScale(const double s) {scale = s;}

	std::vector<double> t, p;
	int nP;
	double arriveTime, finishTime;
	double scale;
	bool keepConst;
};

