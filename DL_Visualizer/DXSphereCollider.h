#pragma once

#include "DXCollider.h"

namespace DX {
	class SphereCollider : public Collider
	{
	public:
		SphereCollider();
		SphereCollider(DirectX::XMFLOAT3 center, float rad);

		virtual bool IsHit(Geometrics::Ray ray, DirectX::XMFLOAT3* hitPt) override;
		void SetScale(DirectX::XMFLOAT3 s) override;

		void Visualize() override;
	protected:
		float rad;
	private:
	};
}
