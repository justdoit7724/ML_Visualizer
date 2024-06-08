
#include "pch.h"
#include "DXCamera.h"
#include "DXTransform.h"
#include "DXGraphic.h"
#include "DXShaderReg.h"

using namespace DX;

#define CAM_SCALE_MIN 0.1
#define CAM_SCALE_MAX 2.3

Camera::Camera(const Graphic* graphic, FRAME_KIND frameKind, float orthoScnWidth, float orthoScnHeight, float n, float f, float verticalViewRad, float aspectRatio, bool skipFrustum)
	:Actor(graphic, ActorKind::Camera), m_scale(1)
{
	m_transform = std::make_unique<Transform>();

	SetView();
	SetFrame(frameKind, XMFLOAT2(orthoScnWidth, orthoScnHeight), n, f, verticalViewRad, aspectRatio);

	frustum.skip = skipFrustum;

	m_cbPos = std::make_unique<Buffer>(graphic->Device(), sizeof(XMFLOAT4));
}
Camera::~Camera()
{
}
void Camera::SetFrame(const FRAME_KIND fKind, XMFLOAT2 orthoSize, const float n, const float f, const float verticalViewRad, const float aspectRatio)
{
	assert(n > 0.0f);

	curFrame = fKind;
	m_size = orthoSize;
	this->n = n;
	this->f = f;
	this->verticalRadian = verticalViewRad;
	this->aspectRatio = aspectRatio;

	SetProj();
}

void DX::Camera::SetScale(float scale)
{
	m_scale = DX::Clamp(CAM_SCALE_MIN, CAM_SCALE_MAX, scale);
}


void Camera::SetView()
{
	XMFLOAT3 pos = m_transform->GetPos();
	XMFLOAT3 forward = m_transform->GetForward();
	XMFLOAT3 up = m_transform->GetUp();
	XMFLOAT3 right = m_transform->GetRight();

	float x = -Dot(pos, right);
	float y = -Dot(pos, up);
	float z = -Dot(pos, forward);

	viewMat = XMMATRIX( // inverse of cam world matrix
		right.x, up.x, forward.x, 0,
		right.y, up.y, forward.y, 0,
		right.z, up.z, forward.z, 0,
		x, y, z, 1);
}

void DX::Camera::SetProj()
{
	switch (curFrame)
	{
	case FRAME_KIND_PERSPECTIVE:
	{
		float sX = 1.0f / (aspectRatio * tan(verticalRadian * m_scale * 0.5f));
		float sY = 1.0f / tan(verticalRadian * m_scale * 0.5f);
		projMat = XMMATRIX(
			sX, 0, 0, 0,
			0, sY, 0, 0,
			0, 0, f / (f - n), 1,
			0, 0, -n * f / (f - n), 0);
	}
	break;
	case FRAME_KIND_ORTHOGONAL:
	{
		float sX = m_scale * 2.0f / m_size.x;
		float sY = m_scale * 2.0f / m_size.y;
		projMat = XMMATRIX(
			sX, 0, 0, 0,
			0, sY, 0, 0,
			0, 0, 1.0f / (f - n), 0,
			0, 0, -n / (f - n), 1
		);
	}
	break;
	}
}

float DX::Camera::GetScale()
{
	return m_scale;
}

void Camera::Update()
{
	if (!frustum.skip)
	{
		XMFLOAT3 p = m_transform->GetPos();
		XMFLOAT3 forward = m_transform->GetForward();
		XMFLOAT3 up = m_transform->GetUp();
		XMFLOAT3 right = m_transform->GetRight();

		float tri = tan(verticalRadian * 0.5f);
		XMFLOAT3 trDir = Normalize(
			right* tri * f * aspectRatio+
			up * tri * f+
			forward* f );
		XMFLOAT3 blDir = Normalize(
			-right* tri * f * aspectRatio +
			-up* tri * f+
			forward* f);

		frustum.front = Geometrics::PlaneInf( p+ forward* f , -forward);
		frustum.back = Geometrics::PlaneInf( p+ forward* n, forward);
		frustum.left = Geometrics::PlaneInf(p, Cross(up, blDir));
		frustum.right = Geometrics::PlaneInf(p, Cross(-up, trDir));
		frustum.top = Geometrics::PlaneInf(p, Cross(right, trDir));
		frustum.bottom = Geometrics::PlaneInf(p, Cross(blDir, right));
	}

	SetView();
	SetProj();

	XMFLOAT3 pos = m_transform->GetPos();
	m_cbPos->Write(m_graphic->DContext(), &pos);
	auto posRes = m_cbPos->Get();
	m_graphic->DContext()->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, &posRes);


}
void Camera::Pick(XMFLOAT2 scnPos, OUT Geometrics::Ray* ray)const
{
	XMFLOAT2 pPos = XMFLOAT2(
		((scnPos.x * 2) / (float)m_size.x - 1),
		-(scnPos.y * 2) / (float)m_size.y + 1);
	XMFLOAT3 vDir = XMFLOAT3(NULL,NULL,NULL);

	const XMFLOAT3 forward = m_transform->GetForward();
	const XMFLOAT3 up = m_transform->GetUp();
	const XMFLOAT3 right = m_transform->GetRight();
	const XMFLOAT3 eye = m_transform->GetPos();
	switch (curFrame)
	{
	case FRAME_KIND_PERSPECTIVE:
	{
		// vPos at z which is on d 
		vDir = Normalize(XMFLOAT3(
			pPos.x * aspectRatio* tan(verticalRadian * 0.5f),
			pPos.y* tan(verticalRadian * 0.5f),
			1));

		XMMATRIX invVDirMat = XMMATRIX(
			right.x, right.y, right.z, 0,
			up.x, up.y, up.z, 0,
			forward.x, forward.y, forward.z, 0,
			eye.x, eye.y, eye.z, 1);
		auto det = XMMatrixDeterminant(VMat());
		invVDirMat = XMMatrixInverse(&det, VMat());

		ray->o = eye;
		ray->d = MultiplyDir(vDir, invVDirMat);
	}
		break;
	case FRAME_KIND_ORTHOGONAL:
	{
		float invSX = m_size.x/2;
		float invSY = m_size.y/2;
		XMFLOAT3 vPos = XMFLOAT3(
			pPos.x * invSX,
			pPos.y * invSY,
			1);
		vDir = Normalize(vPos);

		ray->o =  eye+ right* vPos.x + up* vPos.y;

		ray->d = forward;
	}
		break;
	default:
		break;
	}


	
}
