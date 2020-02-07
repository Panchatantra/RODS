#pragma once

#ifdef __GNUC__
#include <lapacke.h>
#else
#include <mkl.h>
#endif

#include <armadillo>

using namespace arma;

static inline lapack_int eig_sym(const mat &K, const mat &M, vec &omg, mat &Phi)
{
	mat K_ = mat(K);
	mat M_ = mat(M);
	
	lapack_int itype = 1;
	char jobz = 'V';
	char uplo = 'L';
	lapack_int n = (lapack_int)K.n_rows;
	lapack_int lda = n;
	lapack_int ldb = n;

	lapack_int info = LAPACKE_dsygv(LAPACK_COL_MAJOR, itype, jobz, uplo, n, K_.memptr(), lda, M_.memptr(), ldb, omg.memptr());

	Phi = mat(K_);
	omg = sqrt(omg);

	return info;
}

static inline mat expm(const mat &H, const double h)
{
	int N = 20;
	double tau = h/pow(2.0, N);
	mat Ht1 = H*tau;
	mat Ht2 = H*Ht1*tau;
	mat Ht3 = H*Ht2*tau;
	mat Ht4 = H*Ht3*tau;

	mat Ta = Ht1 + Ht2*0.5 + Ht3/6.0 + Ht4/24.0;

	for (int i = 0; i < N; i++)
	{
		Ta = 2.0*Ta + Ta*Ta;
	}

	mat T = eye(size(H)) + Ta;
	return T;
}
