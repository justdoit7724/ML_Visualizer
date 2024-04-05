#pragma once

#include "Component.h"

namespace DX {
	class DXLIB_DLL RasterizerState :
		public Component
	{
	public:
		RasterizerState(ID3D11Device* device, D3D11_RASTERIZER_DESC* desc);
		~RasterizerState();

		void Apply(const Graphic* graphic) override;

	private:
		ID3D11RasterizerState* state;
	};
}

