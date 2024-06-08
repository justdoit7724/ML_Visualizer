
#include "pch.h"
#include "DXBlendState.h"

using namespace DX;

BlendState::BlendState(ID3D11Device* device, D3D11_BLEND_DESC * desc)
{
	if (desc)
	{
		this->desc = *desc;
	}
	else {
		this->desc.AlphaToCoverageEnable = false;
		this->desc.IndependentBlendEnable = false;
		this->desc.RenderTarget[0].BlendEnable = true;
		this->desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		this->desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		this->desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		this->desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		this->desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		this->desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		this->desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	HRESULT hr = device->CreateBlendState(
		&(this->desc),
		&state);

	
	assert(SUCCEEDED(hr));
}

BlendState::~BlendState()
{
	state->Release();
}

void BlendState::Modify(ID3D11Device* device, D3D11_BLEND_DESC * desc)
{
	state->Release();

	HRESULT hr = device->CreateBlendState(desc, &state);
	assert(SUCCEEDED(hr));
}

void BlendState::Apply(const Graphic* graphic) 
{
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	graphic->DContext()->OMSetBlendState(state, blendFactors, 0xffffffff);
}

