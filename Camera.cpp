#include "Camera.h"
#include "WinApp.h"
#include <cassert>

using namespace DirectX;

void Camera::Initialize(Input* input)
{
	assert(input);

	this->input = input;

	eye = {0, distance, -distance};
	target = {0, 0, upRot};
	up = {0, 1, 0};

	nearZ = 0.1f;
	farZ = 2000.f;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::CameraPosInitialize()
{
	eye = {0, distance, -distance};
	target = {0, 0, upRot};
	up = {0, 1, 0};

	nearZ = 0.1f;
	farZ = 2000.f;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::Update()
{
	//平行移動
	//position.x -= move.normalize().x * velocity;
	//matView *= XMMatrixTranslation(position.x,0.f,0.f);

	matViewProjection = matView * matProjection;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	XMFLOAT3 keye{eye.x, eye.y, eye.z};
	XMFLOAT3 ktarget{target.x, target.y, target.z};
	XMFLOAT3 kup{up.x, up.y, up.z};
	matView = XMMatrixLookAtLH(XMLoadFloat3(&keye), XMLoadFloat3(&ktarget), XMLoadFloat3(&kup));
}

void Camera::UpdateProjectionMatrix()
{
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),	//上下画角45°
		(float)WinApp::window_width / WinApp::window_height,			//aspect比(画面横幅/画面縦幅)
		nearZ, farZ				//前端、奥端
	);
}