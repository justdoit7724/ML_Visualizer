
#include "pch.h"

#include "DXRasterizerState.h"

using namespace DX;

RasterizerState::RasterizerState(ID3D11Device* device, D3D11_RASTERIZER_DESC* desc)
{
	D3D11_RASTERIZER_DESC curDesc;

	if (desc == nullptr)
	{
		ZeroMemory(&curDesc, sizeof(D3D11_RASTERIZER_DESC));
		curDesc.FillMode = D3D11_FILL_SOLID;
		curDesc.CullMode = D3D11_CULL_BACK;
		curDesc.FrontCounterClockwise = false;
	}
	else
	{
		curDesc = *desc;
	}

	HRESULT hr = device->CreateRasterizerState(&curDesc, &state);
	assert(SUCCEEDED(hr));
}
RasterizerState::~RasterizerState()
{
	state->Release();
}
void RasterizerState::Apply(const Graphic* graphic)
{
	graphic->DContext()->RSSetState(state);
}
