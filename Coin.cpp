#include "Coin.h"
#include <cassert>

void Coin::Initialize(Model *model, Object3D *modelObject, const float velocity, Vector3 pos)
{
	assert(model);

	this->model = model;
	this->modelObject = modelObject;
	this->velocity = velocity;
	this->position = pos;
	this->rotation = {0,0,0};

	this->modelObject->Initialize();
	this->modelObject->SetModel(this->model);
	this->modelObject->SetPosition(position);

	SetCollisionAttribute(kCollisionAttributeThier);
	SetCollisionMask(kCollisionAttributeMine);
}

void Coin::Update()
{
	if(position.z <= -50.f){
		IsDead = true;
	}

	position.z -= velocity;
	rotation.y += RotationVelocity;

	modelObject->SetPosition(position);
	modelObject->SetRotation(rotation);
	modelObject->Update();
}

void Coin::Draw(ID3D12GraphicsCommandList *commandList)
{
	modelObject->Draw(commandList);
}

void Coin::OnCollision(Collider* TouchCollision)
{
	IsDead = true;
}
