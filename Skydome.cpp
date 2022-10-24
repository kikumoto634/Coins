#include "Skydome.h"
#include <cassert>

using namespace DirectX;
using namespace std;

void Skydome::Initialize(Model* model, Object3D* modelObject, Vector3 pos)
{
	assert(model);

	this->model = model;
	this->modelObject = modelObject;
	this->position = pos;
	this->rotation = {0.f,XMConvertToRadians(0.f),0.f};

	this->modelObject->Initialize();
	this->modelObject->SetModel(this->model);
	this->modelObject->SetPosition(this->position);
	this->modelObject->SetRotation(this->rotation);
}

void Skydome::Update()
{
	/*if(XMConvertToRadians(rotation.y) >= XMConvertToRadians(360.f))
	{
		rotation.y = XMConvertToRadians(0.f);
	}*/
	//rotation.y += XMConvertToRadians(1.f);
	
	//modelObject->SetRotation(rotation);
	modelObject->Update();
}

void Skydome::Draw(ID3D12GraphicsCommandList *commandList)
{
	modelObject->Draw(commandList);
}
