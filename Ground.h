#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"
#include "Collider.h"

#include <memory>

/// <summary>
/// ’n–Ê
/// </summary>
class Ground : public Collider
{
public:
	void Initialize(Model* model, Object3D* modelObject, const float velocity,  Vector3 pos = {0,0,0});
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

	//Õ“Ë”»’è
	void OnCollision(Collider* TouchCollision) override;

	//Get
	const Vector3 GetPosition() override	{return position;}
	const float GetWidth() override	{return width;}
	const float GetHeight()	override	{return height;}
	const std::string GetName() override	{return name;}

	//Set
	void SetVelocity(float velocity)	{this->velocity = velocity;}

private:
	void Movement();

private:
	Model* model = nullptr;
	Object3D* modelObject;

	Vector3 position;
	float width = 90.f;
	float height = 5.f;
	std::string name = "ground";
	float velocity;
};

