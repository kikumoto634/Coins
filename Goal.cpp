#include "Goal.h"

void Goal::Initialize(Model *model, Object3D *modelObject, const float velocity,  Vector3 pos)
{
	assert(model);

	this->model = model;
	this->modelObject = modelObject;
	this->position = pos;
	this->velocity = velocity;

	this->modelObject->Initialize();
	this->modelObject->SetModel(this->model);
	this->modelObject->SetPosition(this->position);

	//Õ“ËÝ’è
	SetCollisionAttribute(kCollisionAttributeThier);
	SetCollisionMask(kCollisionAttributeMine);
}

void Goal::Update()
{
	Movement();

	this->modelObject->SetPosition(position);
	modelObject->Update();
}

void Goal::Draw(ID3D12GraphicsCommandList *commandList)
{
	modelObject->Draw(commandList);
}

void Goal::OnCollision(Collider *TouchCollision)
{
}

void Goal::Movement()
{
	position.z -= velocity;
}
