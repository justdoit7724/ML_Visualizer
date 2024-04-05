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

	void AddTo(Vector& a, double v);
	void SubTo(Vector& a, double v);
	void MulTo(Vector& a, double v);
	void DivTo(Vector& a, double v);
	Vector Mul(const Vector& a, double v);
	
	void AddTo(Matrix& A, const Matrix& B);
	void SubTo(Matrix& A, const Matrix& B);
	Matrix Dot(const Matrix& A, const Matrix& B);
	Matrix Add(const Matrix& A, const Matrix& B);
	Matrix Sub(const Matrix& A, const Matrix& B);
	Matrix Mul(const Matrix& A, const Matrix& B);
	void AddTo(Matrix& A, double v);
	void SubTo(Matrix& A, double v);
	void MulTo(Matrix& A, double v);
	void DivTo(Matrix& A, double v);
	Matrix ToMatrix(Vector v);
	Matrix Identity(int n);

	Matrix Transpose(const Matrix& A);

	double Sigmoid(double v);
	Vector Sigmoid(const Vector& v);

	Vector Exp(Vector v);

	Matrix Zeros(int m, int n, double v=0);
	Matrix Zeros(const Matrix& m, double v = 0);
	

	std::string ToString(const Matrix& m);
}
#endif
