#include "SampleArea.h"

using namespace std;

void SampleArea::AppInitialize(WinApp *winapp, DirectXCommon *dxCommon, Input *input, TextureManager *textureManager, GeometryManager *geometryModel, Camera *camera, DebugText *debugText, SoundManager *soundManager)
{
	this->winApp = winapp;
	this->dxCommon = dxCommon;
	this->input = input;
	this->textureManager = textureManager;
	this->geometryModel = geometryModel;
	this->camera = camera;
	this->debugText = debugText;
	this->soundManager = soundManager;
}

void SampleArea::Initialize()
{
#pragma region オブジェクト初期化

	/// <summary>
	/// プレイヤー
	/// </summary>
	playerModel = make_unique<Model>();
	playerModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("Player"));
	playerObject = make_unique<Object3D>();
	player = make_unique<Player>();
	player->Initialize(this->input, playerModel.get(), playerObject.get(), {0,10,0});

	/// <summary>
	/// 天球
	/// </summary>
	skydomeModel = make_unique<Model>();
	skydomeModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("skydome"));
	skydomeObject = make_unique<Object3D>();
	skydome = make_unique<Skydome>();
	skydome->Initialize(skydomeModel.get(), skydomeObject.get());

	/// <summary>
	/// 地面
	/// </summary>
	groundModel = make_unique<Model>();
	groundModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("Area01"));
	groundObject = make_unique<Object3D>();
	ground = make_unique<Ground>();
	ground->Initialize(groundModel.get(), groundObject.get(), ObjectsVelocity, {0,-10,0});

	/// <summary>
	/// ゴール
	/// </summary>
	goalModel = make_unique<Model>();
	goalModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("goal"));
	goalObject = make_unique<Object3D>();
	goal = make_unique<Goal>();
	goal->Initialize(goalModel.get(), goalObject.get(), ObjectsVelocity, {0,-10,2100});
#pragma endregion 

#pragma region スプライト初期化
	result = make_unique<Result>();

	this->textureManager->LoadTexture(1, L"Resources/BackGround/TransparentBackGround.png");
	this->textureManager->LoadTexture(2, L"Resources/Text/CLEAR_TextFont.png");
	this->textureManager->LoadTexture(3, L"Resources/coinSprite/CoinBack.png");
	this->textureManager->LoadTexture(4, L"Resources/coinSprite/Coin.png");
	this->textureManager->LoadTexture(5, L"Resources/Text/RESULT_TextFont.png");
	this->textureManager->LoadTexture(6, L"Resources/HP/Hart.png");
	this->textureManager->LoadTexture(7, L"Resources/HP/HartBack.png");
	this->textureManager->LoadTexture(8, L"Resources/Button/buttonUp.png");
	this->textureManager->LoadTexture(9, L"Resources/Button/buttonDown.png");
	this->textureManager->LoadTexture(10, L"Resources/Button/buttonLeft.png");
	this->textureManager->LoadTexture(11, L"Resources/Button/buttonRight.png");
	this->textureManager->LoadTexture(12, L"Resources/Button/buttonW.png");
	this->textureManager->LoadTexture(13, L"Resources/Button/buttonS.png");

	//HP
	for(int i = 0; i < player->GetMaxHp(); i++)
	{
		HpSprite[i] = make_unique<Sprite>();
		player->HpSpriteInitialize(HpSprite[i].get(), i);
	}
	//Button
	ButtonUp = make_unique<Sprite>();
	ButtonDown = make_unique<Sprite>();
	ButtonLeft = make_unique<Sprite>();
	ButtonRight = make_unique<Sprite>();
	ButtonW = make_unique<Sprite>();
	ButtonS = make_unique<Sprite>();
	player->BottomSpriteInitialize(ButtonUp.get(), ButtonDown.get(), ButtonLeft.get(), ButtonRight.get(), ButtonW.get(), ButtonS.get());

	//背景
	backGround = make_unique<Sprite>();
	backGround->Initialize(1);
	result->BackGroundSpriteInitialize(backGround.get());

	//リザルト
	RESULTtext = make_unique<Sprite>();
	RESULTtext->Initialize(5);
	result->ResultTextInitialize(RESULTtext.get());

	//クリア
	CLEARtext = make_unique<Sprite>();
	CLEARtext->Initialize(2);
	result->ClearTextInitialie(CLEARtext.get());

	//リスタート
	RESTARTtext = make_unique<Sprite>();
	RESTARTtext->Initialize(5);
	result->RestartTextInitialize(RESTARTtext.get());

	//コイン
	for(int i = 0; i < 3; i++)
	{
		coinSprite[i] = make_unique<Sprite>();
		coinSprite[i]->Initialize(3);
		result->CoinSpriteInitialize(coinSprite[i].get(), i);
	}

#pragma endregion

	//当たり判定
	collisionManager = make_unique<CollisionManager>();
}

void SampleArea::Update()
{
#pragma region オブジェクト 更新
	//プレイヤー 更新
	player->Update();
	result->SetCoinNum(player->GetCoins());
	result->SetIsGoal(player->GetIsGoal());
	if(player->GetHp() <= 0)
	{
		result->SetIsDead(true);
	}

	//天球 更新
	skydome->Update();
	//地面 更新
	ground->Update();
	//ゴール 更新
	goal->Update();
#pragma endregion

#pragma region スプライト
	//プレイヤー
	player->SpriteUpdate(camera->GetMatView(), camera->GetMatProjection());
#pragma endregion

#pragma region 汎用機能 更新
	this->camera->Update();
#pragma endregion

#pragma region シーン 更新

	//リザルト
	GoalUpdate();
	DeadUpdate();

	//シーン遷移
	if(result->GetIsSceneChange())
	{
		if(input->Trigger(DIK_SPACE))
		{
			sceneChange = true;
			sceneChangeNumber = SCENE::SELECT;
		}
	}


	//衝突判定更新
	CheckAllCollision();
#pragma endregion

#ifdef _DEBUG

	if(input->Trigger(DIK_SPACE))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::SELECT;
	}

#endif // _DEBUG
}

void SampleArea::Draw()
{
	this->dxCommon->BeginDraw();

	///3D描画
#pragma region オブジェクト_3D描画
	//プレイヤー 描画
	player->Draw(dxCommon->GetCommandList());

	//天球 描画
	skydome->Draw(dxCommon->GetCommandList());
	//地面 描画
	ground->Draw(dxCommon->GetCommandList());
	//ゴール 描画
	goal->Draw(dxCommon->GetCommandList());
#pragma endregion

	///2D描画
	Sprite::SetPipelineState(this->dxCommon->GetCommandList());
#pragma region メンバ_2D描画
	//Hp
	player->SpriteDraw(this->dxCommon->GetCommandList());

	//リザルト
	result->GoalDraw(this->dxCommon->GetCommandList());
#pragma endregion

#ifdef _DEBUG

#endif // _DEBUG


	this->dxCommon->EndDraw();
}

void SampleArea::Finalize()
{
}


#pragma region 衝突判定
void SampleArea::CheckAllCollision()
{
	//リストクリア
	collisionManager->CollisionClear();
	//リスト追加
	collisionManager->SetCollision(player.get());
	collisionManager->SetCollision(goal.get());

	//総当たり判定
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = collisionManager->colliders.begin();
	for(; itrA != collisionManager->colliders.end(); ++itrA){
		//イテレータAからコライダーAを取得
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for(; itrB != collisionManager->colliders.end(); ++itrB){
			//イテレータBからコライダーBを取得
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void SampleArea::CheckCollisionPair(Collider *colliderA, Collider *colliderB)
{
	//衝突フィルタリング
	if(colliderA->GetCollisionAttribute() != colliderB->GetCollisionMask() || colliderB->GetCollisionAttribute() != colliderA->GetCollisionMask()){
		return;
	}

	if(CheckCollisionDetail(colliderA, colliderB)){
		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);
	}
}

bool SampleArea::CheckCollisionDetail(Collider *colliderA, Collider *colliderB)
{
	//ボックス同士の当たり判定
	if(colliderA->GetPosition().x-colliderA->GetWidth() > colliderB->GetPosition().x+colliderB->GetWidth())		return false;
	if(colliderA->GetPosition().x+colliderA->GetWidth() < colliderB->GetPosition().x-colliderB->GetWidth())		return false;
	if(colliderA->GetPosition().y-colliderA->GetHeight() > colliderB->GetPosition().y+colliderB->GetHeight())	return false;
	if(colliderA->GetPosition().y+colliderA->GetHeight() < colliderB->GetPosition().y-colliderB->GetHeight())	return false;
	if(colliderA->GetPosition().z-colliderA->GetWidth() > colliderB->GetPosition().z+colliderB->GetWidth())		return false;
	if(colliderA->GetPosition().z+colliderA->GetWidth() < colliderB->GetPosition().z-colliderB->GetWidth())		return false;

	return true;
}
#pragma endregion

#pragma region ゴール処理
void SampleArea::GoalUpdate()
{
	//リザルト処理
	result->GoalUpdate();

	if(!result->GetIsGoal()) return ;
	ObjectVelocityStop();
}

void SampleArea::DeadUpdate()
{
	result->GoalUpdate();

	if(!result->GetIsDead()) return ;
	ObjectVelocityStop();
}

void SampleArea::ObjectVelocityStop()
{
	//速度設定
	ground->SetVelocity(0.f);
	goal->SetVelocity(0.f);
}
#pragma endregion