
#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <assert.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <limits>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <fstream>
#include <numeric>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")

#include "MLMathHelp.h"



#define FORWARD DirectX::XMFLOAT3(0,0,1)
#define BACKWARD DirectX::XMFLOAT3(0,0,-1)
#define RIGHT DirectX::XMFLOAT3(1,0,0)
#define LEFT DirectX::XMFLOAT3(-1,0,0)
#define UP DirectX::XMFLOAT3(0,1,0)
#define DOWN DirectX::XMFLOAT3(0,-1,0)
#define ORIGIN DirectX::XMFLOAT3(0,0,0)
#define NOWHERE DirectX::XMFLOAT3(FLT_MAX,FLT_MAX,FLT_MAX)

#define COLOR_RED DirectX::XMFLOAT4(1.0,0.0,0.0,1.0)
#define COLOR_GREEN DirectX::XMFLOAT4(0.0,1.0,0.0,1.0)
#define COLOR_BLUE DirectX::XMFLOAT4(0.0,0.0,1.0,1.0)
#define COLOR_DARK_GRAY DirectX::XMFLOAT4(0.2,0.2,0.2,1.0)
#define DX_EPSILON 1.0e-12

//
//#ifdef NDEBUG
//#pragma comment(lib, __FILE__"\\..\\..\\x64\\Release\\MLLib.lib")
//#pragma comment(lib, __FILE__"\\..\\..\\x64\\Release\\DXLib.lib")
//#else
//#pragma comment(lib, __FILE__"\\..\\..\\x64\\Debug\\MLLib.lib")
//#pragma comment(lib, __FILE__"\\..\\..\\x64\\Debug\\DXLib.lib")
//#endif

using namespace DirectX;

inline void SetText(CEdit* ui, std::string str)
{
	ui->SetWindowTextW(std::wstring(str.begin(), str.end()).c_str());
}
inline std::string GetText(CEdit* ui)
{
	CString cstr;
	ui->GetWindowTextW(cstr);
	std::string str = CT2A(cstr);
	return str;
}
inline std::string GetText(CComboBox* ui)
{
	CString cstr;
	ui->GetLBText(ui->GetCurSel(), cstr);
	std::string str = CT2A(cstr);
	return str;
}
//
//inline std::string ToString(float f, int fracCount = 2)
//{
//
//	std::string ret = std::to_string(f);
//	auto dot = ret.find('.');
//	if (fracCount == 0)
//	{
//		return ret.substr(0, dot);
//	}
//	if (dot != std::string::npos)
//		return ret.substr(0, dot + 1 + fracCount);
//
//	return ret;
//}
//
//inline std::string ToString(DirectX::XMFLOAT3 v)
//{
//	std::string ret = "";
//
//	std::string x = std::to_string(v.x);
//	std::string y = std::to_string(v.y);
//	std::string z = std::to_string(v.z);
//	auto dot = x.find('.');
//	if (dot == std::string::npos)
//	{
//		ret += x;
//	}
//	else
//		ret += x.substr(0, dot + 1 + 2);
//	ret += ", ";
//	dot = y.find('.');
//	if (dot == std::string::npos)
//	{
//		ret += y;
//	}
//	else
//		ret += y.substr(0, dot + 1 + 2);
//	ret += ", ";
//	dot = z.find('.');
//	if (dot == std::string::npos)
//	{
//		ret += z;
//	}
//	else
//		ret += z.substr(0, dot + 1 + 2);
//
//	return ret;
//}
//
//inline std::string ToString(DirectX::XMFLOAT4 v)
//{
//	std::string ret = "";
//
//	std::string x = std::to_string(v.x);
//	std::string y = std::to_string(v.y);
//	std::string z = std::to_string(v.z);
//	std::string w = std::to_string(v.w);
//	auto dot = x.find('.');
//	if (dot == std::string::npos)
//	{
//		ret += x;
//	}
//	else
//		ret += x.substr(0, dot + 1 + 2);
//	ret += ", ";
//	dot = y.find('.');
//	if (dot == std::string::npos)
//	{
//		ret += y;
//	}
//	else
//		ret += y.substr(0, dot + 1 + 2);
//	ret += ", ";
//	dot = z.find('.');
//	if (dot == std::string::npos)
//	{
//		ret += z;
//	}
//	else
//		ret += w.substr(0, dot + 1 + 2);
//	ret += ", ";
//	dot = w.find('.');
//	if (dot == std::string::npos)
//	{
//		ret += w;
//	}
//	else
//		ret += w.substr(0, dot + 1 + 2);
//
//
//	return ret;
//}

inline ML::Vector operator*(const ML::Vector& a, float v)
{
	ML::Vector output;
	for (int i = 0; i < a.size(); ++i)
	{
		output.push_back(a[i] * v);
	}
	return output;
}
inline void operator/=(ML::Vector& a, float v)
{
	for (int i = 0; i < a.size(); ++i)
	{
		a[i] /= v;
	}
}
inline void operator/=(ML::Matrix& a, float v)
{
	for (int i = 0; i < a.size(); ++i)
	{
		for (int j = 0; j < a[i].size(); ++j)
		{
			a[i][j] /= v;
		}
	}
}
inline ML::Vector operator +(const ML::Vector& a, const ML::Vector& b)
{
	assert(a.size() == b.size());

	ML::Vector output;
	for (int i = 0; i < a.size(); ++i)
	{
		output.push_back(a[i] + b[i]);
	}
	return output;
}
inline ML::Vector operator-(const ML::Vector& a, const ML::Vector& b)
{
	assert(a.size() == b.size());

	ML::Vector output;
	for (int i = 0; i < a.size(); ++i)
	{
		output.push_back(a[i] - b[i]);
	}
	return output;
}
inline ML::Vector operator/(const ML::Vector& a, const ML::Vector& b)
{
	assert(a.size() == b.size());

	ML::Vector output;
	for (int i = 0; i < a.size(); ++i)
	{
		output.push_back(a[i] / b[i]);
	}
	return output;
}
inline void operator-=(ML::Vector& a, const ML::Vector& b)
{
	assert(a.size() == b.size());

	for (int i = 0; i < a.size(); ++i)
	{
		a[i] -= +b[i];
	}
}
//
//std::string ToString(const ML::Vector& v, int fracCount=2)
//{
//	std::string output;
//	for (int j = 0; j < v.size(); ++j)
//	{
//		output += ToString(v[j], fracCount) + " ";
//	}
//	return output;
//}
//std::string ToString(const ML::Matrix& m, int fracCount=2)
//{
//	std::string output;
//
//	for (int i = 0; i < m.size(); ++i)
//	{
//		output += ToString(m[i], fracCount) + "\n";
//	}
//
//	return output;
//}



inline DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 output;
	output.x = a.x + b.x;
	output.y = a.y + b.y;
	output.z = a.z + b.z;
	return output;
}

inline XMFLOAT3 operator-(const XMFLOAT3& a)
{
	return XMFLOAT3(-a.x, -a.y, -a.z);
}
inline DirectX::XMFLOAT3 operator -(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 output;
	output.x = a.x - b.x;
	output.y = a.y - b.y;
	output.z = a.z - b.z;
	return output;
}

inline DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 v, float f)
{
	DirectX::XMFLOAT3 output;
	output.x = v.x * f;
	output.y = v.y * f;
	output.z = v.z * f;
	return output;
}

inline DirectX::XMFLOAT3 operator/(DirectX::XMFLOAT3 v, float f)
{
	DirectX::XMFLOAT3 sum;
	sum.x = v.x / f;
	sum.y = v.y / f;
	sum.z = v.z / f;
	return sum;
}
inline DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 div;
	div.x = a.x / b.x;
	div.y = a.y / b.y;
	div.z = a.z / b.z;
	return div;
}

inline float SqrLength(DirectX::XMFLOAT3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}


inline std::wstring GetEXEPath()
{
	wchar_t* str = new wchar_t[400];
	GetModuleFileName(NULL, str, 400);

	auto path = std::wstring(str) + L"/../";

	delete[] str;

	return path;
}

#endif //PCH_H
