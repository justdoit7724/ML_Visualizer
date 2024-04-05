#pragma once
#include "Component.h"

namespace DX
{
	class DXLIB_DLL BlendState : public Component
	{
	public:
		BlendState(ID3D11Device* device, D3D11_BLEND_DESC* desc);
		~BlendState();

		void Modify(ID3D11Device* device, D3D11_BLEND_DESC* desc);
		void Apply(const Graphic* graphic) override;


	private:
		D3D11_BLEND_DESC desc;
		ID3D11BlendState* state;
	};
}

