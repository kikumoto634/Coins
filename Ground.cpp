#include "Ground.h"
#include <cassert>

using namespace std;

void Ground::Initialize(Model *model, Object3D *modelObject, float velocity, Vector3 pos)
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
	//SetCollisionAttribute(kCollisionAttributeDamage);
	//SetCollisionMask(kCollisionAttributeMine);
}

void Ground::Update()
{
	Movement();

	this->modelObject->SetPosition(position);
	modelObject->Update();
}

void Ground::Draw(ID3D12GraphicsCommandList *commandList)
{
	modelObject->Draw(commandList);
}

void Ground::OnCollision(Collider* TouchCollision)
{
}

void Ground::Movement()
{
	position.z -= velocity;
}

