#pragma once

#include <DirectXMath.h>
#include <vector>

namespace DX {

	/*
	DirectX::XMFLOAT2 operator+(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b)
	{
		return DirectX::XMFLOAT2(a.x + b.x, a.y + b.y);
	}

	DirectX::XMFLOAT2 operator-(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b)
	{
		return DirectX::XMFLOAT2(a.x - b.x, a.y - b.y);
	}

	DirectX::XMFLOAT2 operator+(DirectX::XMFLOAT2 v, float f)
	{
		return DirectX::XMFLOAT2(v.x + f, v.y + f);
	}

	DirectX::XMFLOAT2 operator-(DirectX::XMFLOAT2 v, float f)
	{
		return DirectX::XMFLOAT2(v.x - f, v.y - f);
	}

	DirectX::XMFLOAT2 operator*(DirectX::XMFLOAT2 v, float f)
	{
		return DirectX::XMFLOAT2(v.x * f, v.y * f);
	}

	DirectX::XMFLOAT2 operator/(DirectX::XMFLOAT2 v, float f)
	{
		return DirectX::XMFLOAT2(v.x / f, v.y / f);
	}

	void operator+=(DirectX::XMFLOAT3& a, DirectX::XMFLOAT3 b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
	}

	DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
	{
		DirectX::XMFLOAT3 sum;
		sum.x = a.x - b.x;
		sum.y = a.y - b.y;
		sum.z = a.z - b.z;
		return sum;
	}

	DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 v)
	{
		DirectX::XMFLOAT3 sum;
		sum.x = -v.x;
		sum.y = -v.y;
		sum.z = -v.z;
		return sum;
	}

	DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 v, float f)
	{
		DirectX::XMFLOAT3 sum;
		sum.x = v.x + f;
		sum.y = v.y + f;
		sum.z = v.z + f;
		return sum;
	}

	DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 v, float f)
	{
		DirectX::XMFLOAT3 sum;
		sum.x = v.x - f;
		sum.y = v.y - f;
		sum.z = v.z - f;
		return sum;
	}


	DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
	{
		DirectX::XMFLOAT3 sum;
		sum.x = a.x * b.x;
		sum.y = a.y * b.y;
		sum.z = a.z * b.z;
		return sum;
	}


	void operator/=(XMFLOAT4& v, float f)
	{
		v.x /= f;
		v.y /= f;
		v.z /= f;
		v.w /= f;
	}

	bool operator==(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b)
	{
		return (a.x == b.x && a.y == b.y);
	}

	bool operator!=(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b)
	{
		return (a.x != b.x || a.y != b.y);
	}

	bool operator==(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
	{
		return (a.x == b.x && a.y == b.y && a.z == b.z);
	}

	bool operator!=(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
	{
		return (a.x != b.x || a.y != b.y || a.z != b.z);
	}

	bool operator==(XMFLOAT4 a, XMFLOAT4 b)
	{
		return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
	}

	bool operator!=(XMFLOAT4 a, XMFLOAT4 b)
	{
		return (a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w);
	}

	XMFLOAT4 operator*(XMFLOAT4 v, float f)
	{
		return XMFLOAT4(v.x * f, v.y * f, v.z * f, v.w * f);
	}
	*/

	bool  Equal(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b);
	bool  Equal(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
	
	

	float  Clamp(float a, float b, float v);
	float  Lerp(float v1, float v2, float t);
	float  Rand01();
	float  Length(DirectX::XMFLOAT2 v);
	DirectX::XMFLOAT2  Cross(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b);
	float  Dot(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b);
	DirectX::XMFLOAT3  Multiply(DirectX::XMFLOAT3 v, const DirectX::XMMATRIX& m);
	DirectX::XMFLOAT3  MultiplyDir(DirectX::XMFLOAT3 v, const DirectX::XMMATRIX& m);
	float  Length(DirectX::XMFLOAT3 v);
	DirectX::XMFLOAT3  Cross(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
	DirectX::XMFLOAT3  Lerp(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2, float t);
	float  Dot(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
	DirectX::XMFLOAT3  Normalize(DirectX::XMFLOAT3 v);
	DirectX::XMFLOAT3  Abs(DirectX::XMFLOAT3 v);
	float  Max(DirectX::XMFLOAT3 v);
	// radian between dir1, dir2
	float  Rad(DirectX::XMFLOAT3 d1, DirectX::XMFLOAT3 d2);
	DirectX::XMFLOAT3  RotateFromDir(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 dir, float rad);

	DirectX::XMFLOAT4  Multiply(DirectX::XMFLOAT4 v, const DirectX::XMMATRIX& m);

	DirectX::XMFLOAT3  GetColorScale(float t);
	DirectX::XMFLOAT3  GetGrayScale(float t);

	const float PI = 3.141592;
}

