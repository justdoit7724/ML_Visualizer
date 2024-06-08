#include "pch.h"
#include "DXWorldGraphic.h"
#include "DXCamera.h"
#include "DXTransform.h"
#include "DXLight.h"

using namespace DX;

WorldGraphic::WorldGraphic(HWND hwnd, int msaa)
	:Graphic(hwnd, msaa)
{
	Actor* tmp;
	CreateActor(ActorKind::Camera, &tmp);
	SetMainCamera((Camera*)tmp);
	Camera* cam = (Camera*)tmp;
	cam->GetTrasform()->SetTranslation(0, 0, -10);
	CreateActor(ActorKind::Light_Direction, &tmp);
	DirectionalLight* light;
	light = (DirectionalLight*)tmp;
	light->SetDir(Normalize(XMFLOAT3(1, -1, 1)));


}
