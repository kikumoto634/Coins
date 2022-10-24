#include "Obstacle.h"

void Obstacle::Initialize(Model *model, Object3D *modelObject, const float velocity, Vector3 pos)
{
	assert(model);

	this->model = model;
	this->modelObject = modelObject;
	this->velocity = velocity;
	this->position = pos;

	this->modelObject->Initialize();
	this->modelObject->SetModel(this->model);
	this->modelObject->SetPosition(position);

	//è’ìÀê›íË
	SetCollisionAttribute(kCollisionAttributeThier);
	SetCollisionMask(kCollisionAttributeMine);
}

void Obstacle::Update()
{
	if(position.z <= -50.f){
		IsDead = true;
	}

	position.z -= velocity;

	modelObject->SetPosition(position);
	modelObject->Update();
}

void Obstacle::Draw(ID3D12GraphicsCommandList *commandList)
{
	modelObject->Draw(commandList);
}

void Obstacle::OnCollision(Collider* TouchCollision)
{
}
