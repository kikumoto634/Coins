#include "Tutorial01.h"
#include <fstream>
#include <cassert>

using namespace std;

void Tutorial01::AppInitialize(WinApp *winapp, DirectXCommon *dxCommon, Input *input, TextureManager *textureManager, GeometryManager *geometryModel, Camera *camera, DebugText *debugText, SoundManager *soundManager)
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

void Tutorial01::Initialize()
{
#pragma region オブジェクト初期化

	/// <summary>
	/// カメラ
	/// </summary>
	this->camera->CameraPosInitialize();

	/// <summary>
	/// プレイヤー
	/// </summary>
	playerModel = make_unique<Model>();
	playerModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("Player"));
	playerObject = make_unique<Object3D>();
	player = make_unique<Player>();
	player->Initialize(this->input, playerModel.get(), playerObject.get(), {0,10,-0});

	/// <summary>
	/// コイン
	/// </summary>
	LoadCoinPopData();
	UpdateCoinPopCommands();

	/// <summary>
	/// 天球
	/// </summary>
	skydomeModel = make_unique<Model>();
	skydomeModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("skydome"));
	skydomeObject = make_unique<Object3D>();
	skydome = make_unique<Skydome>();
	skydome->Initialize(skydomeModel.get(), skydomeObject.get());
	skydomeRot = {0,0,0};

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
	this->textureManager->LoadTexture(8, L"Resources/Button/buttonW.png");
	this->textureManager->LoadTexture(9, L"Resources/Button/buttonS.png");
	this->textureManager->LoadTexture(10, L"Resources/Button/buttonA.png");
	this->textureManager->LoadTexture(11, L"Resources/Button/buttonD.png");
	this->textureManager->LoadTexture(12, L"Resources/Button/buttonUp.png");
	this->textureManager->LoadTexture(13, L"Resources/Button/buttonDown.png");
	this->textureManager->LoadTexture(14, L"Resources/ScoreIncrement/CoinScoreIncrement.png");
	this->textureManager->LoadTexture(15, L"Resources/ShockEffect/ShockEffect.png");

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

	//コイン加算UI
	coinIncrement = make_unique<Sprite>();
	player->CoinIncrementSpriteInitialize(coinIncrement.get());
	
	//ダメージエフェクト
	damageEffect = make_unique<Sprite>();
	player->DamageEffectSpriteInitialize(damageEffect.get());

#pragma endregion

	//当たり判定
	collisionManager = make_unique<CollisionManager>();
}

void Tutorial01::Update()
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

	//コイン
	//削除
	coin.remove_if([](unique_ptr<Coin>& obj){
		return obj->GetIsDead();
	});
	for(unique_ptr<Coin>& obj : coin){
		obj->Update();
	}

	//天球 更新
	skydomeRot += {0,XMConvertToRadians(skydomeRotSp),0};
	skydomeObject->SetRotation(skydomeRot);
	skydomeObject->Update();
	skydome->Update();
	//地面 更新
	ground->Update();
	//ゴール 更新
	goal->Update();
#pragma endregion

#pragma region スプライト
	//HP更新
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
		if(input->Trigger(DIK_SPACE) || input->Trigger(DIK_RETURN))
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

	this->debugText->Printf(0,0,1.f,"PlayerPos: X:%f, Y:%f, Z:%f", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
	//this->debugText->Printf(0,16,1.f,"SpritePos: X:%f, Y:%f", positionreticle.x, positionreticle.y);
	this->debugText->Printf(0,32,1.f,"PlayerHP:%d", player->GetHp());
	this->debugText->Printf(0,48,1.f,"Coin:%d", player->GetCoins());
	this->debugText->Printf(0,64,1.f,"Goal:%d", result->GetIsGoal());
	this->debugText->Printf(0,80,1.f,"Dead:%d", result->GetIsDead());

#endif // _DEBUG
}

void Tutorial01::Draw()
{
	this->dxCommon->BeginDraw();

	///3D描画
#pragma region オブジェクト_3D描画
	//プレイヤー 描画
	player->Draw(dxCommon->GetCommandList());

	//コイン
	for(unique_ptr<Coin>& obj : coin){
		obj->Draw(this->dxCommon->GetCommandList());
	}

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
	//UI
	player->SpriteDraw(this->dxCommon->GetCommandList());
	//リザルト
	result->GoalDraw(this->dxCommon->GetCommandList());
#pragma endregion

#ifdef _DEBUG
	this->debugText->DrawAll(this->dxCommon->GetCommandList());
#endif // _DEBUG


	this->dxCommon->EndDraw();
}

void Tutorial01::Finalize()
{
}

#pragma region コイン処理
void Tutorial01::LoadCoinPopData()
{
	//file開く
	ifstream file;
	file.open("Resources/csv/Tutorial01/CoinPopData.csv");
	assert(file.is_open());

	//file培養を文字列ストリームにコピー
	coinPopCommands << file.rdbuf();
	file.close();
}
void Tutorial01::UpdateCoinPopCommands()
{
	//一行分の文字列を入れる変数
	string line;

	//コマンドループ
	while(getline(coinPopCommands, line)){
		//一行分のっ文字列をストリームに変換して解析しやすく
		istringstream line_stream(line);

		string word;
		//区切りで行の銭湯文字列取得
		getline(line_stream, word, ',');

		//"//"から始まるのはcomment
		if(word.find("//") == 0){
			//飛ばす
			continue;
		}

		//POPコマンド
		if(word.find("POP") == 0){
			//X座標
			getline(line_stream, word, ',');
			float x = (float)atof(word.c_str());

			//Y座標
			getline(line_stream, word, ',');
			float y = (float)atof(word.c_str());

			//Z座標
			getline(line_stream, word, ',');
			float z = (float)atof(word.c_str());

			//POP
			CoinPop(Vector3(x,y,z));
		}
	}
}
void Tutorial01::CoinPop(Vector3 pos)
{
	unique_ptr<Model> newcoinModel = make_unique<Model>();
	newcoinModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("coin"));
	unique_ptr<Object3D> newcoinObject = make_unique<Object3D>();
	unique_ptr<Coin> newcoin = make_unique<Coin>();
	newcoin->Initialize(newcoinModel.get(), newcoinObject.get(), ObjectsVelocity, pos);

	coin.push_back(move(newcoin));
	coinModel.push_back(move(newcoinModel));
	coinObject.push_back(move(newcoinObject));
}
#pragma endregion

#pragma region 衝突判定
void Tutorial01::CheckAllCollision()
{
	//リストクリア
	collisionManager->CollisionClear();
	//リスト追加
	collisionManager->SetCollision(player.get());
	collisionManager->SetCollision(goal.get());
	for(const unique_ptr<Coin>& obj : coin){
		collisionManager->SetCollision(obj.get());
	}

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

void Tutorial01::CheckCollisionPair(Collider *colliderA, Collider *colliderB)
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

bool Tutorial01::CheckCollisionDetail(Collider *colliderA, Collider *colliderB)
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
void Tutorial01::GoalUpdate()
{
	//リザルト処理
	result->GoalUpdate();

	if(!result->GetIsGoal()) return ;
	ObjectVelocityStop();
}

void Tutorial01::DeadUpdate()
{
	result->GoalUpdate();

	if(!result->GetIsDead()) return ;
	ObjectVelocityStop();
}

void Tutorial01::ObjectVelocityStop()
{
	//速度設定
	ground->SetVelocity(0.f);
	goal->SetVelocity(0.f);
	for(const unique_ptr<Coin>& obj : coin){
		obj->SetVelocity(0.f);
	}
}
#pragma endregion

