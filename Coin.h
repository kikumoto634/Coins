#pragma once
#include "Vector3.h"
#include "Model.h"
#include "Object3D.h"
#include "Collider.h"
#include <string>

//score—p
class Coin : public Collider
{
private:
	const float DeletePositionZ = -50.f;
	const float RotationVelocity = 2.5f;

public:
	void Initialize(Model* model, Object3D* modelObject, const float velocity, Vector3 pos = {0,0,0});
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

	//Õ“Ë”»’è
	void OnCollision(Collider* TouchCollision) override;

	//Get
	const Vector3 GetPosition() override	{return position;}
	const float GetWidth() override	{return width;}
	const float GetHeight()	override	{return height;}
	const std::string GetName() override	{return name;}
	bool GetIsDead() const {return IsDead;}

	//Set
	void SetVelocity(float velocity)	{this->velocity = velocity;}

private:

	Model* model = nullptr;
	Object3D* modelObject;

	Vector3 position;
	Vector3 rotation;
	float width =  10.f;
	float height = 10.f;
	std::string name = "coin";

	float velocity;

	bool IsDead = false;
};

