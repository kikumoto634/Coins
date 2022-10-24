#include "Player.h"
#include <cassert>

using namespace std;

void Player::Initialize(Input* input, Model* model, Object3D* modelObject, Vector3 pos, Vector3 rot)
{
	assert(input);
	assert(model);

	this->input = input;
	this->model = model;
	this->modelObject = modelObject;
	position = pos;
	rotation = rot;
	scale = {1,1,1};

	velocity = velocity_Movement;
	hp = MaxHp;

	this->modelObject->Initialize();
	this->modelObject->SetModel(this->model);
	this->modelObject->SetPosition(this->position);
	this->modelObject->SetRotation(this->rotation);
	this->modelObject->SetScale(this->scale);

	//衝突属性設定
	SetCollisionAttribute(kCollisionAttributeMine);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionAttributeThier);
}

void Player::HpSpriteInitialize(Sprite *hp, int indices)
{
	hpSprite[indices] = hp;
	hpSprite[indices]->Initialize(6);
	hpSprite[indices]->SetAnchorpoint({0.5f, 0.5f});
	hpSprite[indices]->SetPosition({64 + (float)indices * 52, 48});
	hpSprite[indices]->SetSize({48, 48});
	hpSprite[indices]->SetTextureRect(0,0,256,256);
	hpSprite[indices]->SetColor({1,1,1,1});
}

void Player::BottomSpriteInitialize(Sprite * up, Sprite* down, Sprite* left, Sprite* right, Sprite* w, Sprite* s)
{
	//↑
	buttonUp = up;
	buttonUp->Initialize(8);
	buttonUp->SetAnchorpoint({0.5f,0.5f});
	buttonUp->SetPosition({155, 575});//115
	buttonUp->SetSize({64,64});
	buttonUp->SetTextureRect(0,0,256,256);
	buttonUp->SetColor({1,1,1,1});

	//↓
	buttonDown = down;
	buttonDown->Initialize(9);
	buttonDown->SetAnchorpoint({0.5f,0.5f});
	buttonDown->SetPosition({155, 655});
	buttonDown->SetSize({64,64});
	buttonDown->SetTextureRect(0,0,256,256);
	buttonDown->SetColor({1,1,1,1});

	//←
	buttonLeft = left;
	buttonLeft->Initialize(10);
	buttonLeft->SetAnchorpoint({0.5f,0.5f});
	buttonLeft->SetPosition({75, 655});
	buttonLeft->SetSize({64,64});
	buttonLeft->SetTextureRect(0,0,256,256);
	buttonLeft->SetColor({1,1,1,1});

	//→
	buttonRight = right;
	buttonRight->Initialize(11);
	buttonRight->SetAnchorpoint({0.5f,0.5f});
	buttonRight->SetPosition({235, 655});
	buttonRight->SetSize({64,64});
	buttonRight->SetTextureRect(0,0,256,256);
	buttonRight->SetColor({1,1,1,1});

	//W
	buttonW = w;
	buttonW->Initialize(12);
	buttonW->SetAnchorpoint({0.5f,0.5f});
	buttonW->SetPosition({1125, 575});//1125
	buttonW->SetSize({64,64});
	buttonW->SetTextureRect(0,0,256,256);
	buttonW->SetColor({1,1,1,1});

	//S
	buttonS = s;
	buttonS->Initialize(13);
	buttonS->SetAnchorpoint({0.5f,0.5f});
	buttonS->SetPosition({1125, 655});
	buttonS->SetSize({64,64});
	buttonS->SetTextureRect(0,0,256,256);
	buttonS->SetColor({1,1,1,1});
}

void Player::CoinIncrementSpriteInitialize(Sprite *coinIncrement)
{
	this->coinIncrement = coinIncrement;
	coinIncrement->Initialize(14);
	coinIncrement->SetPosition({640, 360});
	coinIncrement->SetTextureRect(0,0,96,71);
	coinIncrement->SetAnchorpoint({0.5f, 0.5f});
	coinIncrement->SetColor({1,1,1,coinAlpha});
	coinIncrement->SetIsInvisible(true);
}

void Player::DamageEffectSpriteInitialize(Sprite *damageEffect)
{
	this->damageEffect = damageEffect;
	damageEffect->Initialize(15);
	damageEffect->SetSize({256, 256});
	damageEffect->SetTextureRect(0,0,damageEffectWidth, 240);
	damageEffect->SetAnchorpoint({0.5f, 0.5f});
	damageEffect->SetIsInvisible(true);
}

void Player::Update()
{
	//ダメージ時間解除
	DamageTime();
	DamageMotion();

	//入力
	Movement();
	Squat();
	Jump();

	Goal();
	Dead();


	//更新
	modelObject->SetPosition(position);
	modelObject->SetRotation(rotation);
	modelObject->SetScale(scale);
	modelObject->Update();
}

void Player::SpriteUpdate(DirectX::XMMATRIX matView, DirectX::XMMATRIX matViewProjection)
{
	this->matView = matView;
	this->matViewProjection = matViewProjection;

	HpSpriteUpdate();
	ButtonSpriteUpdate();
	CoinIncrementUpdate();
	DamageEffectUpdate();
}

void Player::Draw(ID3D12GraphicsCommandList* commandList)
{
	modelObject->Draw(commandList);
}

void Player::SpriteDraw(ID3D12GraphicsCommandList* commandList)
{
	//HP
	for(Sprite* sp : hpSprite){
		sp->Draw(commandList);
	}

	//button
	buttonUp->Draw(commandList);
	buttonDown->Draw(commandList);
	buttonLeft->Draw(commandList);
	buttonRight->Draw(commandList);
	buttonW->Draw(commandList);
	buttonS->Draw(commandList);

	//コイン加算UI
	coinIncrement->Draw(commandList);

	//ダメージエフェクト
	damageEffect->Draw(commandList);
}

void Player::OnCollision(Collider* TouchCollision)
{
	//ダメージ無
	if(TouchCollision->GetName() == "coin"){
		coins = coins + 1;
		coinIncrement->SetIsInvisible(false);
		coinAlpha= 1.f;
		return ;
	}
	if(TouchCollision->GetName() == "goal"){
		IsGoal = true;
		return ;
	}

	//ダメージあり
	if(TouchCollision->GetName() == "wall" || TouchCollision->GetName() == "slideWall"){
		if(!IsDamage && hp != 0){
			IsDamageEffect = true;
			damageEffect->SetIsInvisible(false);
		}
	}

	Damage();
}

void Player::Movement()
{
	if(IsGoal) return ;
	if(hp <= 0) return;

	position += MovementInput().normalize() * velocity;
	const float LimitPositionX = 90.f;
	const float MinPositionZ = -50.f;
	const float MaxPositionZ = 200.f;
	position.x = max(position.x, -LimitPositionX);
	position.x = min(position.x,  LimitPositionX);
	position.z = max(position.z,  MinPositionZ);
	position.z = min(position.z,  MaxPositionZ);
}

Vector3 Player::MovementInput()
{
	if(!IsDamage)rotation.y = 0.f;

	kmove = {0, 0, 0};
	IsbuttonUp = false;
	IsbuttonDown = false;
	IsbuttonLeft = false;
	IsbuttonRight = false;

	if(input->Push(DIK_D))
	{
		if(!IsDamage)rotation.y = 45.f;

		kmove.x += 1.f;
		IsbuttonRight = true;
	}
	else if(input->Push(DIK_A))
	{
		if(!IsDamage)rotation.y = -45.f;

		kmove.x -= 1.0f;
		IsbuttonLeft = true;
	}

	if(input->Push(DIK_W))
	{
		kmove.z += 1.0f;
		IsbuttonUp = true;
	}
	else if(input->Push(DIK_S))
	{
		kmove.z -= 1.0f;
		IsbuttonDown = true;
	}

	return kmove;
}

void Player::HpSpriteUpdate()
{
	for(Sprite* sp : hpSprite){
		if(sp->GetTexNumber() == 7)	continue;

		//hpアニメーション処理
		Vector2 size = sp->GetSize();
		//切り替え
		if(!IsHpAnim && size.x >= 58.f){
			size = {58.f,58.f};
			IsHpAnim = true;
		}
		else if(IsHpAnim && size.x <= 48.f){
			size = {48.f, 48.f};
			IsHpAnim = false;
		}

		//サイズ
		if(!IsHpAnim){
			size += {0.5f,0.5f};
		}
		else if(IsHpAnim){
			size -= {0.25f, 0.25f};
		}
		sp->SetSize(size);
	}

	if(!hpSpriteDelete)  return;

	hpSprite[hp]->Initialize(7);
	hpSprite[hp]->SetAnchorpoint({0.5f, 0.5f});
	hpSprite[hp]->SetPosition({64 + (float)hp * 52, 48});
	hpSprite[hp]->SetSize({48.f, 48.f});
	hpSprite[hp]->SetTextureRect(0,0,256,256);
	hpSprite[hp]->SetColor({1,1,1,1});
	hpSpriteDelete = false;
}

void Player::ButtonSpriteUpdate()
{
	buttonUp->SetColor({1,1,1,1});
	buttonDown->SetColor({1,1,1,1});
	buttonLeft->SetColor({1,1,1,1});
	buttonRight->SetColor({1,1,1,1});
	buttonW->SetColor({1,1,1,1});
	buttonS->SetColor({1,1,1,1});

	if(IsbuttonUp)
	{
		buttonUp->SetColor({0,0,1,1});
	}
	if(IsbuttonDown)
	{
		buttonDown->SetColor({0,0,1,1});
	}
	if(IsbuttonLeft)
	{
		buttonLeft->SetColor({0,0,1,1});
	}
	if(IsbuttonRight)
	{
		buttonRight->SetColor({0,0,1,1});
	}
	if(IsbuttonW)
	{
		buttonW->SetColor({0,0,1,1});
	}
	if(IsbuttonS)
	{
		buttonS->SetColor({0,0,1,1});
	}
}

void Player::CoinIncrementUpdate()
{
	if(!coinIncrement->GetIsInvisible()){

		//時間
		coinUItime += 1.f/60;
		if(coinUItime >= 1.5f){
			coinIncrement->SetIsInvisible(true);
			coinUItime = 0.f;
		}

		//上移動
		coinUIpos = coinIncrement->GetPosition();
		coinUIpos.y -= 1.f;
		coinIncrement->SetPosition(coinUIpos);

		//フェードアウト
		coinAlpha -= 1.f/120;
		coinIncrement->SetColor({1,1,1,coinAlpha});
	}

	//3D -> 2D 座標変換
	if(coinIncrement->GetIsInvisible()){
		//コイン加算UI
		DirectX::XMMATRIX matViewport = 
		{
			WinApp::window_width/2, 0						, 0, 0,
			0					  , -WinApp::window_height/2, 0, 0,
			0					  , 0						, 1, 0, 
			WinApp::window_width/2, WinApp::window_height/2 , 0, 1,
		};
		DirectX::XMMATRIX matViewProjectionViewPort = matView * matViewProjection * matViewport;
		Vector3 positionreticle = Vector3Transform(position, matViewProjectionViewPort);
		coinIncrement->SetPosition(Vector2(positionreticle.x, positionreticle.y));
	}
}

void Player::DamageEffectUpdate()
{
	//座標変換
	DirectX::XMMATRIX matViewport = 
	{
		WinApp::window_width/2, 0						, 0, 0,
		0					  , -WinApp::window_height/2, 0, 0,
		0					  , 0						, 1, 0, 
		WinApp::window_width/2, WinApp::window_height/2 , 0, 1,
	};
	DirectX::XMMATRIX matViewProjectionViewPort = matView * matViewProjection * matViewport;
	Vector3 positionreticle = Vector3Transform(position, matViewProjectionViewPort);
	damageEffect->SetPosition(Vector2(positionreticle.x, positionreticle.y));

	if(IsDamageEffect){
		damageEffectSheet++;
		damageEffect->SetTextureRect(damageEffectSheet*damageEffectWidth, 0, 240, 240);

		if(damageEffectSheet < MaxDamageEffectSheet) return;

		damageEffect->SetIsInvisible(true);
		damageEffectSheet = 0;
		IsDamageEffect = false;
	}
}

void Player::Squat()
{
	if(IsGoal) return ;
	//スクワットから戻る
	if(IsSquat){
		IsbuttonS = false;
		IsSquat = false;
		velocity = velocity_Movement;
		position.y += squatDifferenceY;
		scale.y = 1.f;
		height = NormalHeight;
	}

	//スクワット
	if(SquatInput()){
		IsbuttonS = true;
		IsSquat = true;
		velocity = velocity_SquatMovement;
		position.y -= squatDifferenceY;
		scale.y = 0.5f;
		height = SquatHeight;
	}
}

bool Player::SquatInput()
{
	return input->Push(DIK_DOWN);
}

void Player::Jump()
{
	if(IsGoal) return ;

	if(input->Push(DIK_UP))
	{
		jumpPos[0] = {position.x, postion_Initialize_Y, position.z};
		jumpPos[1] = {position.x, postion_Initialize_Y + jumpHeight, position.z};
		jumpPos[2] = {position.x, postion_Initialize_Y, position.z};

		IsbuttonW = true;
		IsJump = true;
	}

	if(!IsJump) return;

	jumpSecond += 1.f/JumpSecond;

	float a = (1 - jumpSecond);

	position.y = a*a*jumpPos[0].y + 2.f*a*jumpSecond*jumpPos[1].y + jumpSecond*jumpSecond*jumpPos[2].y;

	if(jumpSecond < 1) return;

	position.y = jumpPos[2].y;

	jumpSecond = 0.f;

	IsbuttonW = false;
	IsJump = false;
}

void Player::DamageTime()
{
	if(IsDamage){
		damageWaitTime++;
		if(damageWaitTime > DamageWaitTimeMax){
			IsDamage = false;
			damageWaitTime = 0;
			velocity = velocity_Movement;
			rotation.y = 0.f;
		}
	}
}

void Player::Damage()
{
	if(hp <= 0) return;

	if(!IsDamage)
	{
		hp = hp - 1;
		hpSpriteDelete = true;
		IsDamage = true;
	}
}

void Player::DamageMotion()
{
	if(IsDamage){
		rotation.y += 6;
		velocity = velocity_Movement/2.f;
		
	}
}

void Player::Dead()
{
	if(scale.x <= 0) return;
	if(scale.y <= 0) return;
	if(scale.z <= 0) return;
	if(hp > 0) return;

	scale -= {0.025f, 0.025f, 0.025f};
}

void Player::Goal()
{
	if(!IsGoal) return ;
	if(position.z >= 1000.f) return ;
	rotation.y = 0.f;
	position.z += velocity;
}
