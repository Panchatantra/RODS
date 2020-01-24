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