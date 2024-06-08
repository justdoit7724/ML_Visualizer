#pragma once
#include <vector>
#include <string>

#ifndef _MLLIB_MATHHELP
#define _MLLIB_MATHHELP

namespace ML {

	typedef std::vector<double> Vector;
	typedef std::vector<std::vector<double>> Matrix;


	Vector BinaryMul(const Vector& a, const Vector& b);
	void MulTo(Vector& a, const Vector& b);
	double Dot(const Vector& a, const Vector& b);
	Matrix Dot(const Matrix& A, const Matrix& B);
	Matrix Mul(const Matrix& A, const Matrix& B);
	
	Matrix ToMatrix(Vector v);
	Matrix Identity(int n);

	Matrix Transpose(const Matrix& A);

	double Sigmoid(double v);
	Vector Sigmoid(const Vector& v);

	Vector Exp(Vector v);

	Matrix Zeros(int m, int n, double v=0);
	Matrix Zeros(const Matrix& m, double v = 0);
}
#endif
