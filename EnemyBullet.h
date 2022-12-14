#pragma once
#include "Vector3.h"
#include "GeometryObject3D.h"
#include "Collider.h"

class EnemyBullet : public Collider
{
public:
	void Initialize(UINT textureNumber, GeometryObject3D* object, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

	//衝突判定
	void OnCollision() override;

	bool GetIsDead() const {return isDead;}
	const Vector3 GetPosition() override	{return position;}
	const float GetWidth() override	{return width;}
	const float GetHeight()	override	{return height;}

private:
	static const int32_t kLifeTime = 60 * 5;

private:
	Vector3 position;
	Vector3 rotation;
	GeometryObject3D* object;
	UINT textureNumber;

	float width = 5.f;
	float height = 5.f;

	//速度
	Vector3 velocity;

	//デスタイマー
	int32_t deathTimer = kLifeTime;
	//デスフラグ
	bool isDead = false;
};

