#include "pch.h"
#include "MLMathHelp.h"

using namespace ML;

Vector ML::BinaryMul(const Vector& a, const Vector& b)
{
	assert(a.size() == b.size());

	Vector output;
	for (int i = 0; i < a.size(); ++i)
	{
		output.push_back(a[i] * b[i]);
	}
	return output;
}

void ML::MulTo(Vector& a, const Vector& b)
{
	assert(a.size() == b.size());

	for (int i = 0; i < a.size(); ++i)
	{
		a[i] *= b[i];
	}
}

double ML::Dot(const Vector& a, const Vector& b)
{
	assert(a.size() == b.size());
	double ret = 0;
	for (int i = 0; a.size(); ++i)
	{
		ret += a[i] * b[i];
	}

	return ret;
}

ML::Matrix ML::Dot(const Matrix& A, const Matrix& B)
{
	int hA = A.size();
	int wA = A[0].size();
	int hB = B.size();
	int wB = B[0].size();

	assert(wA == hB);

	Matrix output(hA, Vector(wB, 0));

	for (int x = 0; x < wB; x++) //2
	{
		for (int y = 0; y < hA; ++y) //1
		{
			double dotV = 0;
			for (int z = 0; z < wA; ++z)
			{
				dotV += A[y][z] * B[z][x];
			}
			output[y][x] = dotV;
		}
	}

	return output;
}


Matrix ML::Mul(const Matrix& A, const Matrix& B)
{
	assert(A.size() == B.size() && A[0].size() == B[0].size());

	Matrix output(A.size(), Vector(A[0].size()));

	for (int y = 0; y < A.size(); y++)
	{
		for (int x = 0; x < A[0].size(); ++x)
		{
			output[y][x] = A[y][x] * B[y][x];
		}
	}

	return output;
}

Matrix ML::ToMatrix(Vector v)
{
	Matrix out;
	out.push_back(v);
	
	return Transpose(out);
}

Matrix ML::Identity(int n)
{
	Matrix out(n, Vector(n, 0));
	
	for (int i = 0; i < n; ++i)
	{
		out[i][i] = 1;
	}

	return out;
}


Matrix ML::Transpose(const Matrix& A)
{
	int m = A.size();
	int n = A[0].size();

	Matrix output = Zeros(n, m);

	for (int y = 0; y < m; ++y)
	{
		for (int x = 0; x < n; ++x)
		{
			output[x][y] = A[y][x];
		}
	}
	
	return output;
}

double ML::Sigmoid(double v)
{
	return 1.0 / (1.0 + exp(-v));
}

ML::Vector ML::Sigmoid(const Vector& v)
{
	Vector output(v.size());

	for (int i = 0; i < v.size(); ++i)
	{
		output[i] = Sigmoid(v[i]);
	}

	return output;
}

Vector ML::Exp(Vector v)
{
	Vector output(v.size());
	for (int i = 0; i < v.size(); ++i)
		output[i] = exp(v[i]);

	return output;
}

Matrix ML::Zeros(int m, int n, double v)
{
	return Matrix(m, Vector(n,v));
}

Matrix ML::Zeros(const Matrix& m, double v)
{
	return Zeros(m.size(), m[0].size(), v);
}


std::string ToStringSingle(float f, int fracCount = 2)
{

	std::string ret = std::to_string(f);
	if (f >= 0)
		ret.insert(ret.begin(), ' ');
	auto dot = ret.find('.');
	if (fracCount == 0)
	{
		return ret.substr(0, dot);
	}
	if (dot != std::string::npos)
		return ret.substr(0, dot + 1 + fracCount);

	return ret;
}