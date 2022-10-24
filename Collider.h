#pragma once
#include "Vector3.h"
#include "CollisionConfig.h"
#include <string>

class Collider
{
public:
	//衝突時
	virtual void OnCollision(Collider* TouchCollision) = 0;

	//Get
	virtual const float GetWidth() = 0;
	virtual const float GetHeight() = 0;
	virtual const std::string GetName() = 0;
	virtual const Vector3 GetPosition() = 0;

	const uint32_t GetCollisionAttribute()	{return collisionAttribute;}
	const uint32_t GetCollisionMask()	{return collisionMask;}

	void SetCollisionAttribute(const uint32_t bute)	{this->collisionAttribute = bute;}
	void SetCollisionMask(const uint32_t bute)	{this->collisionMask = bute;}

private:
	//衝突半径
	float Width = 5.f;//デフォルト値
	float Height = 5.f;
	std::string name = "unknown";

	//衝突属性(自陣)
	uint32_t collisionAttribute = 0xffffffff;
	//衝突マスク(敵)
	uint32_t collisionMask = 0xffffffff;
};

