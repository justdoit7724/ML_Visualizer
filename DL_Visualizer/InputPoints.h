#pragma once
#include <DirectXMath.h>
#include <vector>
#include "DXMathHelper.h"
#include "MLMathHelp.h"

class InputPoints
{
public:
	const ML::Matrix GetData()const;
	const ML::Matrix GetLabel(bool multi=false)const;
	DirectX::XMFLOAT3 GetMinPt()const;
	DirectX::XMFLOAT3 GetMaxPt()const;
	DirectX::XMFLOAT4 GetColor(ML::Vector pred);
	const int m_nLabel;

protected:
	InputPoints(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 size, int nLabel);
	ML::Matrix m_x;
	ML::Matrix m_y;
	DirectX::XMFLOAT3 m_origin;
	DirectX::XMFLOAT3 m_size;

	std::vector<DirectX::XMFLOAT4> m_labelCol;

private: 
	InputPoints() = delete;
};

