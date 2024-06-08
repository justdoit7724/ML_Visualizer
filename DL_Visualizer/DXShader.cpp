
#include "pch.h"

#include "DXShader.h"
#include "DXBuffer.h"
#pragma comment(lib, "D3DCompiler.lib")

using namespace DX;



Shader::~Shader()
{
	for (auto cb : cbs)
	{
		delete cb.second.data;
	}
	for (auto samp : samps)
	{
		if(samp.second.data)
			samp.second.data->Release();
	}
}

void Shader::AddCB(ID3D11Device* device, UINT slot, UINT arrayNum, UINT byteSize)
{
	if (cbs.find(slot) == cbs.end())
	{
		cbs.insert(std::pair<UINT, BindingCB>(slot, BindingCB(new Buffer(device,byteSize), arrayNum)));
	}
	else
	{
		// slot overlap
		assert(false);
	}
}

void Shader::AddSRV(UINT slot, UINT arrayNum)
{
	assert(srvs.find(slot) == srvs.end());

	srvs.insert(std::pair<UINT, BindingSRV>(slot, BindingSRV(nullptr, arrayNum)));
}

void Shader::AddSamp(ID3D11Device* device, UINT slot, UINT arrayNum, D3D11_SAMPLER_DESC * desc)
{
	if (samps.find(slot) == samps.end())
	{
		ID3D11SamplerState* newSampler=nullptr;
		device->CreateSamplerState(desc, &newSampler);

		samps.insert(std::pair<UINT, BindingSamp>(slot, BindingSamp(newSampler, arrayNum)));
	}
	else
	{
		// slot overlap
		assert(false);
	}
}

void Shader::WriteCB(ID3D11DeviceContext* dContext, UINT slot, const void * data)
{
	if (cbs.find(slot)!=cbs.end() && data)
	{
		cbs[slot].data->Write(dContext, data);
	}
	else
	{
		//no reg
		assert(false);
	}
}

void Shader::WriteSRV(UINT slot, ID3D11ShaderResourceView* srv)
{
	assert(srvs.find(slot) != srvs.end());

	srvs[slot].data = srv;
}

void Shader::RemoveCB(UINT slot)
{
	assert(cbs.find(slot) != cbs.end());

	cbs.erase(slot);
}

bool Shader::CheckCBSlot(UINT slot)
{
	return cbs.count(slot);
}




VShader::VShader(ID3D11Device* device, std::string fileName, const D3D11_INPUT_ELEMENT_DESC * layoutDesc, UINT layoutNum)
{
	ID3DBlob* vsBlob;


	std::wstring finalPath = GetEXEPath() + std::wstring(fileName.begin(), fileName.end());
	HRESULT hr = D3DReadFileToBlob(
		finalPath.c_str(),
			&vsBlob);
	assert(SUCCEEDED(hr));
	hr = device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		&vs);
	assert(SUCCEEDED(hr));

	hr = device->CreateInputLayout(
			layoutDesc,
			layoutNum,
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			&iLayout);
	assert(SUCCEEDED(hr));
	vsBlob->Release();

}

VShader::~VShader()
{
	if(iLayout)
		iLayout->Release();
	if(vs)
		vs->Release();
}


void VShader::Apply(const Graphic* graphic)
{
	graphic->DContext()->IASetInputLayout(iLayout);
	graphic->DContext()->VSSetShader(vs, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		auto res = i->second.data->Get();
		graphic->DContext()->VSSetConstantBuffers(i->first, i->second.arrayNum, &res);
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		graphic->DContext()->VSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		graphic->DContext()->VSSetSamplers(slot, arrayNum, &samp);
	}
}

void DX::VShader::UnboundSRVAll(ID3D11DeviceContext* dContext) const
{
	for (auto it = srvs.begin(); it != srvs.end(); ++it)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		dContext->VSSetShaderResources(it->first, it->second.arrayNum, &nullSRV);
	}
}


GShader::GShader(ID3D11Device* device, std::string fileName)
{
	if (fileName != "")
	{
		std::wstring wGS(fileName.begin(), fileName.end());
		ID3DBlob* blob;

		HRESULT hr = D3DReadFileToBlob(
			(GetEXEPath()+wGS).c_str(),
				&blob);
		assert(SUCCEEDED(hr));
		hr = device->CreateGeometryShader(
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				nullptr,
				&gs);
		assert(SUCCEEDED(hr));
	}
}

GShader::~GShader()
{
	if(gs)
		gs->Release();
}

void GShader::Apply(const Graphic* graphic)
{
	if (gs)
	{
		graphic->DContext()->GSSetShader(gs, nullptr, 0);

		for (auto i = cbs.begin(); i != cbs.end(); ++i)
		{
			auto res = i->second.data->Get();
			graphic->DContext()->GSSetConstantBuffers(i->first, i->second.arrayNum, &res);
		}
		for (auto i = srvs.begin(); i != srvs.end(); ++i)
		{
			UINT slot = i->first;
			UINT arrayNum = i->second.arrayNum;
			ID3D11ShaderResourceView* srv = i->second.data;

			graphic->DContext()->GSSetShaderResources(slot, arrayNum, &srv);
		}
		for (auto i = samps.begin(); i != samps.end(); ++i)
		{
			UINT slot = i->first;
			UINT arrayNum = i->second.arrayNum;
			ID3D11SamplerState* samp = i->second.data;

			graphic->DContext()->GSSetSamplers(slot, arrayNum, &samp);
		}
	}
	else
	{
		graphic->DContext()->GSSetShader(nullptr, nullptr, 0);
	}
}

void DX::GShader::UnboundSRVAll(ID3D11DeviceContext* dContext) const
{
	for (auto it = srvs.begin(); it != srvs.end(); ++it)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		dContext->GSSetShaderResources(it->first, it->second.arrayNum, &nullSRV);
	}
}

PShader::PShader(ID3D11Device* device, std::string fileName)
{
	if (fileName != "")
	{
		std::wstring wPS(fileName.begin(), fileName.end());
		ID3DBlob* psBlob;
		HRESULT hr = D3DReadFileToBlob(
			(GetEXEPath()+wPS).c_str(),
			&psBlob);
		assert(SUCCEEDED(hr));
		hr = device->CreatePixelShader(
				psBlob->GetBufferPointer(),
				psBlob->GetBufferSize(),
				nullptr,
				&ps);
		assert(SUCCEEDED(hr));
		psBlob->Release();
	}
}
PShader::~PShader()
{
	if(ps)
		ps->Release();
}
void PShader::Apply(const Graphic* graphic)
{
	graphic->DContext()->PSSetShader(ps, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		auto res = i->second.data->Get();
		graphic->DContext()->PSSetConstantBuffers(i->first, i->second.arrayNum, &res);
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		graphic->DContext()->PSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		graphic->DContext()->PSSetSamplers(slot, arrayNum, &samp);
	}
}

void DX::PShader::UnboundSRVAll(ID3D11DeviceContext* dContext) const
{
	for (auto it = srvs.begin(); it != srvs.end(); ++it)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		dContext->PSSetShaderResources(it->first, it->second.arrayNum, &nullSRV);
	}
}

CShader::CShader(ID3D11Device* device, const std::string CSfileName)
{
	std::wstring wCS(CSfileName.begin(), CSfileName.end());
	ID3DBlob* csBlob;

	HRESULT hr=	D3DReadFileToBlob(
		(GetEXEPath()+wCS).c_str(),
			&csBlob);
	assert(SUCCEEDED(hr));
	hr = device->CreateComputeShader(
			csBlob->GetBufferPointer(),
			csBlob->GetBufferSize(),
			nullptr,
			&cs);
	assert(SUCCEEDED(hr));
	csBlob->Release();
}

CShader::~CShader()
{
	if(cs)
		cs->Release();
}

void CShader::Apply(const Graphic* graphic)
{
	graphic->DContext()->CSSetShader(cs, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		auto res = i->second.data->Get();
		graphic->DContext()->CSSetConstantBuffers(i->first, i->second.arrayNum,&res);
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		graphic->DContext()->CSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		graphic->DContext()->CSSetSamplers(slot, arrayNum, &samp);
	}
}

void DX::CShader::UnboundSRVAll(ID3D11DeviceContext* dContext) const
{
	for (auto it = srvs.begin(); it != srvs.end(); ++it)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		dContext->CSSetShaderResources(it->first, it->second.arrayNum, &nullSRV);
	}
}

HShader::HShader(ID3D11Device* device, std::string fileName)
{
	if (fileName == "")
		return;

	std::wstring wCS(fileName.begin(), fileName.end());
	ID3DBlob* blob;

	HRESULT hr =D3DReadFileToBlob(
		(GetEXEPath()+wCS).c_str(),
			&blob);
	assert(SUCCEEDED(hr));

	hr=device->CreateHullShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&hs);
	assert(SUCCEEDED(hr));
}

HShader::~HShader()
{
	if(hs)
		hs->Release();
}

void HShader::Apply(const Graphic* graphic)
{
	graphic->DContext()->HSSetShader(hs, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		auto res= i->second.data->Get();
		graphic->DContext()->HSSetConstantBuffers(i->first, i->second.arrayNum, &res);
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		graphic->DContext()->HSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		graphic->DContext()->HSSetSamplers(slot, arrayNum, &samp);
	}
}

void DX::HShader::UnboundSRVAll(ID3D11DeviceContext* dContext) const
{
	for (auto it = srvs.begin(); it != srvs.end(); ++it)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		dContext->HSSetShaderResources(it->first, it->second.arrayNum, &nullSRV);
	}
}

DShader::DShader(ID3D11Device* device, std::string fileName)
{
	if (fileName == "")
		return;

	std::wstring wCS(fileName.begin(), fileName.end());
	ID3DBlob* blob;

	HRESULT hr = D3DReadFileToBlob(
		(GetEXEPath()+wCS).c_str(),
			&blob);
	assert(SUCCEEDED(hr));

	hr = device->CreateDomainShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&ds);
	assert(SUCCEEDED(hr));
	blob->Release();
}

DShader::~DShader()
{
	if(ds)
		ds->Release();
}

void DShader::Apply(const Graphic* graphic)
{
	graphic->DContext()->DSSetShader(ds, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		auto res = i->second.data->Get();
		graphic->DContext()->DSSetConstantBuffers(i->first, i->second.arrayNum, &res);
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		graphic->DContext()->DSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		graphic->DContext()->DSSetSamplers(slot, arrayNum, &samp);
	}
}

void DX::DShader::UnboundSRVAll(ID3D11DeviceContext* dContext) const
{
	for (auto it = srvs.begin(); it != srvs.end(); ++it)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		dContext->DSSetShaderResources(it->first, it->second.arrayNum, &nullSRV);
	}
}
