#include "pch.h"
#include "InputPoints.h"

InputPoints::InputPoints(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 size, int nLabel)
	:m_origin(origin), m_size(size), m_nLabel(nLabel)
{
	m_labelCol.push_back({ 1,0,0,1 });
	m_labelCol.push_back({ 0,0,1,1 });
	m_labelCol.push_back({ 0,1,0,1 });
	m_labelCol.push_back({ 1,1,0,1 });
	m_labelCol.push_back({ 0,1,1,1 });
	m_labelCol.push_back({ 1,0,1,1 });
}

const ML::Matrix InputPoints::GetData()const
{
	return m_x;
}


void ConvertToMulti(ML::Matrix& y)
{
	int nLabel = 0;
	int m = y.size();
	for (int i = 0; i < m; ++i)
	{
		nLabel = max(nLabel, y[i][0]);
	}
	nLabel++;

	for (int i = 0; i < m; ++i)
	{
		int label = y[i][0];
		y[i][0] = 1;
		for (int j = 0; j < label; ++j)
		{
			y[i].insert(y[i].begin(), 0);
		}
		while (y[i].size() < nLabel)
			y[i].push_back(0);
	}
}

const ML::Matrix InputPoints::GetLabel(bool multi)const
{
	if (multi)
	{
		auto output = m_y;
		ConvertToMulti(output);
		return output;
	}

	return m_y;
}

DirectX::XMFLOAT3 InputPoints::GetMinPt()const
{
	return m_origin;
}

DirectX::XMFLOAT3 InputPoints::GetMaxPt()const
{
	auto output = m_origin;
	output.x += m_size.x;
	output.y += m_size.y;
	output.z += m_size.z;

	return output;
}

DirectX::XMFLOAT4 InputPoints::GetColor(ML::Vector pred)
{
	int n = pred.size();
	DirectX::XMFLOAT4 mCol = { 0,0,0,1 };
	if (n > 1)
	{
		for (int i = 0; i < n; ++i)
		{
			double t = pred[i];
			if (t < 1)
			{
				t = 1.0 / (1 + exp(-20*(t-0.75)));
			}
			else
			{
				t= 1- 1.0 / (1 + exp(-20 * (t - 1.25)));
			}

			mCol.x += t * m_labelCol[i].x;
			mCol.y += t * m_labelCol[i].y;
			mCol.z += t * m_labelCol[i].z;
		}

	}
	else
	{
		double t = cos(pred[0] * 20 / DirectX::XM_PI) / 2 + 0.5;
		auto col = DX::GetColorScale(t);
		int label = min(m_nLabel - 1, max(0, (int)(pred[0] + 0.5)));
		mCol = m_labelCol[label];
		mCol.x *= t;
		mCol.y *= t;
		mCol.z *= t;
	}



	return mCol;
}
