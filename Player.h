#pragma once
#include <memory>
#include <list>
#include <vector>

#include "Model.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Vector3.h"
#include "Input.h"
#include "Collider.h"
#include <string>

class Player : public Collider
{
private:
	//変数
	const float velocity_Movement = 2.5f;
	const float velocity_SquatMovement = 1.5f;
	static const int MaxHp = 3;
	const int DamageWaitTimeMax = 60;

	const float NormalHeight = 8.f;
	const float SquatHeight = 5.f;

public:
	void Initialize(Input* input, Model* model, Object3D* modelObject, Vector3 pos = {0,0,0}, Vector3 rot = {0,0,0});
	void HpSpriteInitialize(Sprite* hp, int indices);
	void BottomSpriteInitialize(Sprite* up, Sprite* down, Sprite* left, Sprite* right, Sprite* w, Sprite* s);
	void CoinIncrementSpriteInitialize(Sprite* coinIncrement);
	void DamageEffectSpriteInitialize(Sprite* damageEffect);

	void Update();
	void SpriteUpdate(DirectX::XMMATRIX matView, DirectX::XMMATRIX matViewProjection);
	void Draw(ID3D12GraphicsCommandList* commandList);
	void SpriteDraw(ID3D12GraphicsCommandList* commandList);

	//衝突判定
	void OnCollision(Collider* TouchCollision) override;

	//Get
	const Vector3 GetPosition() override	{return position;}
	const float GetWidth() override	{return width;}
	const float GetHeight()	override	{return height;}
	const std::string GetName() override	{return name;}
	
	const Vector3 GetMove()	{return kmove;}
	const float GetMovementSp()	{return velocity_Movement;}
	const int GetMaxHp()	{return MaxHp;}
	const int GetHp()	{return hp;}
	const uint32_t GetCoins()	{return coins;}
	const bool GetIsGoal()	{return IsGoal;}

private:
	void Movement();
	Vector3 MovementInput();

	void HpSpriteUpdate();
	void ButtonSpriteUpdate();
	void CoinIncrementUpdate();

	void Squat();
	bool SquatInput();

	void Jump();

	void DamageTime();
	void Damage();
	void DamageMotion();
	void DamageEffectUpdate();

	void Dead();
	void Goal();

private:
	//オブジェクト
	Model* model = nullptr;
	Object3D* modelObject;
	Input* input = nullptr;

	//HP
	//Sprite* hpSprite[MaxHp];
	std::vector<Sprite*> hpSprite = {nullptr, nullptr, nullptr};
	bool hpSpriteDelete = false;
	bool IsHpAnim = false;

	//button
	Sprite* buttonUp;
	Sprite* buttonDown;
	Sprite* buttonLeft;
	Sprite* buttonRight;
	Sprite* buttonW;
	Sprite* buttonS;
	bool IsbuttonUp = false;
	bool IsbuttonDown = false;
	bool IsbuttonLeft = false;
	bool IsbuttonRight = false;
	bool IsbuttonW = false;
	bool IsbuttonS = false;

	//コイン加算
	Sprite* coinIncrement;
	DirectX::XMMATRIX matView;
	DirectX::XMMATRIX matViewProjection;
	Vector2 coinUIpos;
	float coinAlpha = 1.f;
	float coinUItime = 0.f;
	bool IsCoinUIDelete = false;

	//情報
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	float velocity;

	//ダメージ
	int hp = MaxHp;
	bool IsDamage = false;
	int damageWaitTime = 0u;
	
	//ダメージ演出
	Sprite* damageEffect;
	bool IsDamageEffect = false;
	const float damageEffectWidth = 240;
	const int MaxDamageEffectSheet = 12;
	int damageEffectSheet = 0;


	bool IsSquat = false;
	float squatDifferenceY = 5.f;

	bool IsJump = false;
	Vector3 jumpPos[3];
	const float postion_Initialize_Y = 10.f;
	const float jumpHeight = 100.f;
	const float second = 60.f;
	const float JumpSecond = 1.0f * second;
	float jumpSecond = 0.f;


	bool IsGoal = false;

	uint32_t coins = 0u;

	//サイズ
	float width = 8.f;
	float height = NormalHeight;
	std::string name = "player";

	//移動方向
	Vector3 kmove;
};

