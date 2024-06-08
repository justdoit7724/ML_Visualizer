
#include "pch.h"

#include "DXGraphic.h"
#include "DXLight.h"
#include "DXShaderFormat.h"
#include "DXCamera.h"

#include "DXTransform.h"
#include "DXObject.h"
#include "DXShader.h"
#include "DXBlendState.h"
#include "DXRasterizerState.h"
#include "DXShaderReg.h"
#include "DXDepthStencilState.h"
#include "DXBuffer.h"

using namespace DX;

DirectionalLight::DirectionalLight(const Graphic* graphic, int id, XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float intensity, XMFLOAT3 dir)
	:Light(graphic, id, ActorKind::Light_Direction)
{
	D3D11_BUFFER_DESC cb_desc;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.ByteWidth = sizeof(SHADER_DIRECTIONAL_LIGHT);
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;

	m_cb = std::make_unique<Buffer>(m_graphic->Device(), &cb_desc,nullptr);

	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (m_data.enabled[i].x == LIGHT_DISABLED)
		{
			id = i;
			break;
		}
	}

	SetAmbient(a);
	SetDiffuse(d);
	SetSpecular(s);
	SetDir(dir);
	SetIntensity(intensity);
	Enable(true);

}


void DirectionalLight::SetAmbient(const XMFLOAT3 & a)
{
	ambient = a;
	m_data.ambient[m_index] = XMFLOAT4(a.x, a.y, a.z, 0);
}

void DirectionalLight::SetDiffuse(const XMFLOAT3 & d)
{
	diffuse = d;
	m_data.diffuse[m_index] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void DirectionalLight::SetSpecular(const XMFLOAT3 & s)
{
	specular = s;
	m_data.specular[m_index] = XMFLOAT4(s.x, s.y, s.z, 0);
}

void DX::DirectionalLight::SetIntensity(float i)
{
	m_data.intensity[m_index] = XMFLOAT4(i, i, i, i);
}

void DirectionalLight::SetDir(XMFLOAT3 d)
{
	m_data.dir[m_index] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void DirectionalLight::Enable(bool enable)
{
	if(enable)
		m_data.enabled[m_index] = XMFLOAT4(LIGHT_ENABLED, LIGHT_ENABLED, LIGHT_ENABLED, LIGHT_ENABLED);
	else
		m_data.enabled[m_index] = XMFLOAT4(LIGHT_DISABLED, LIGHT_DISABLED, LIGHT_DISABLED, LIGHT_DISABLED);
}


XMFLOAT3 DirectionalLight::GetDir()const
{
	XMFLOAT4 dir = m_data.dir[m_index];

	return XMFLOAT3(dir.x, dir.y, dir.z);
}

void DirectionalLight::Update()
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	ZeroMemory(&mappedData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_cb->Write(m_graphic->DContext(), &m_data);

	auto res = m_cb->Get();
	m_graphic->DContext()->PSSetConstantBuffers(SHADER_REG_CB_DIRECTIONAL_LIGHT, 1, &res);
}

PointLight::PointLight(const Graphic* graphic, int id, XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float intensity, XMFLOAT3 att, XMFLOAT3 pos)
	:Light(graphic,id, ActorKind::Light_Point)
{
	D3D11_BUFFER_DESC cb_desc;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.ByteWidth = sizeof(SHADER_POINT_LIGHT);
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;

	m_cb = std::make_unique<Buffer>(m_graphic->Device(), &cb_desc,nullptr);



	/*
	set id to 0 for this project, bc separated device
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (m_data.info[i].x == LIGHT_DISABLED)
		{
			id = i;
			break;
		}
	}
	*/


	SetAmbient(a);
	SetDiffuse(d);
	SetSpecular(s);
	SetPos(pos);
	SetIntensity(intensity);
	SetAtt(att);
	Enable(true);
}


void PointLight::SetAmbient(const XMFLOAT3 & a)
{
	ambient = a;
	m_data.ambient[m_index] = XMFLOAT4(a.x, a.y, a.z, 0);
}

void PointLight::SetDiffuse(const XMFLOAT3 & d)
{
	diffuse = d;
	m_data.diffuse[m_index] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void PointLight::SetSpecular(const XMFLOAT3 & s)
{
	specular = s;
	m_data.specular[m_index] = XMFLOAT4(s.x, s.y, s.z, 0);
}

void DX::PointLight::SetIntensity(float i)
{
	m_data.intensity[m_index] = XMFLOAT4(i, i, i, i);
}

void PointLight::SetPos(XMFLOAT3 p)
{
	m_data.pos[m_index] = XMFLOAT4(p.x, p.y, p.z, 0);
}


void PointLight::SetAtt(XMFLOAT3 at)
{
	att = at;
	m_data.att[m_index] = XMFLOAT4(at.x, at.y, at.z, 0);
}

void PointLight::Enable(bool enable)
{
	if(enable)
		m_data.info[m_index].x = LIGHT_ENABLED;
	else
		m_data.info[m_index].x = LIGHT_DISABLED;

}

XMFLOAT3 DX::PointLight::GetPos()
{
	return XMFLOAT3(m_data.pos[m_index].x, m_data.pos[m_index].y, m_data.pos[m_index].z);
}

void PointLight::Update()
{
	HRESULT hr;

	m_cb->Write(m_graphic->DContext(), &m_data);

	auto res = m_cb->Get();
	m_graphic->DContext()->PSSetConstantBuffers(SHADER_REG_CB_POINT_LIGHT, 1, &res);
}

SpotLight::SpotLight(const Graphic* graphic, int id, XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float r, float spot, float intensity, float rad, XMFLOAT3 att, XMFLOAT3 pos, XMFLOAT3 dir)
	:Light(graphic, id, ActorKind::Light_Spot)
{
	D3D11_BUFFER_DESC cb_desc;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.ByteWidth = sizeof(SHADER_SPOT_LIGHT);
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	m_cb = std::make_unique<Buffer>(m_graphic->Device(), &cb_desc,nullptr);

	SetAmbient(a);
	SetDiffuse(d);
	SetSpecular(s);
	SetPos(pos);
	SetDir(dir);
	SetRange(r);
	SetRad(rad);
	SetSpot(spot);
	SetAtt(att);
	SetIntensity(intensity);
	Enable(true);
}


void SpotLight::SetAmbient(const XMFLOAT3 & a)
{
	ambient = a;
	m_data.ambient[m_index] = XMFLOAT4(ambient.x, ambient.y, ambient.z, 0);
}

void SpotLight::SetDiffuse(const XMFLOAT3 & d)
{
	diffuse = d;
	m_data.diffuse[m_index] = XMFLOAT4(diffuse.x, diffuse.y, diffuse.z, 0);
}

void SpotLight::SetSpecular(const XMFLOAT3 & s)
{
	specular = s;
	m_data.specular[m_index] = XMFLOAT4(specular.x, specular.y, specular.z, 0);
}

void DX::SpotLight::SetIntensity(float i)
{
	m_data.intensity[m_index] = XMFLOAT4(i, i, i, i);
}

void SpotLight::SetPos(XMFLOAT3 p)
{
	m_data.pos[m_index] = XMFLOAT4(p.x, p.y, p.z, 0);
}

void SpotLight::SetDir(XMFLOAT3 d)
{
	m_data.dir[m_index] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void SpotLight::SetRange(float r)
{
	range = r;
	m_data.info[m_index].y = r;
}

void SpotLight::SetRad(float r)
{
	rad = r;
	m_data.info[m_index].z = r;
}


void SpotLight::SetSpot(float s)
{
	spot = s;
	m_data.info[m_index].w = spot;
}

void SpotLight::SetAtt(XMFLOAT3 at)
{
	att = at;
	m_data.att[m_index] = XMFLOAT4(att.x, att.y,att.z, 0);
}

void SpotLight::Enable(bool enable)
{
	if (enable)
		m_data.info[m_index].x = LIGHT_ENABLED;
	else
		m_data.info[m_index].x = LIGHT_DISABLED;
}

XMFLOAT3 DX::SpotLight::GetPos()
{
	return XMFLOAT3(m_data.pos[m_index].x, m_data.pos[m_index].y, m_data.pos[m_index].z);
}

void DX::SpotLight::Update()
{
	HRESULT hr;

	m_cb->Write(m_graphic->DContext(), &m_data);

	auto res = m_cb->Get();
	m_graphic->DContext()->PSSetConstantBuffers(SHADER_REG_CB_SPOT_LIGHT, 1, &res);
}

DX::Light::Light(const Graphic* graphic,int index, ActorKind lightKind)
	:Actor(graphic, lightKind), m_index(index)
{
}
