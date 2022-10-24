#include "Select.h"

using namespace std;

void Select::AppInitialize(WinApp *winapp, DirectXCommon *dxCommon, Input *input, TextureManager *textureManager, GeometryManager *geometryModel, Camera *camera, DebugText *debugText, SoundManager *soundManager)
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

void Select::Initialize()
{
	areaNumber = 1;

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
	groundObject->SetScale({0.3f,0.3f,0.3f});
	ground = make_unique<Ground>();
	ground->Initialize(groundModel.get(), groundObject.get(), 0.f, {0,-20,0});

	camera->SetEye({0,100,-200});

#pragma region スプライト初期化
	this->textureManager->LoadTexture(1, L"Resources/SelectArea/arrow.png");
	this->textureManager->LoadTexture(2, L"Resources/SelectArea/AreaNumber/AreaNumber01.png");
	this->textureManager->LoadTexture(3, L"Resources/SelectArea/AreaNumber/AreaNumber02.png");
	this->textureManager->LoadTexture(4, L"Resources/SelectArea/AreaNumber/AreaNumber03.png");
	this->textureManager->LoadTexture(5, L"Resources/SelectArea/AreaNumber/AreaNumber04.png");
	this->textureManager->LoadTexture(6, L"Resources/SelectArea/AreaNumber/AreaNumber05.png");
	this->textureManager->LoadTexture(7, L"Resources/SelectArea/AreaNumber/AreaNumber06.png");
	this->textureManager->LoadTexture(8, L"Resources/SelectArea/AreaNumber/AreaNumber07.png");
	this->textureManager->LoadTexture(9, L"Resources/SelectArea/AreaNumber/AreaNumber08.png");
	this->textureManager->LoadTexture(10, L"Resources/SelectArea/AreaNumber/AreaNumber09.png");
	this->textureManager->LoadTexture(11, L"Resources/EventBar/EventBar.png");

	//矢印
	ArrowInitialize();

	//ナンバー
	AreaNumberInitialize();

	//イベント演出
	for(int i = 0; i < 2; i++){
		eventBarSprite[i] = make_unique<Sprite>();
		eventBarSprite[i]->Initialize(11);
		eventBarSprite[i]->SetAnchorpoint({0.5f,0.5f});
		eventBarSprite[i]->SetSize({1280, 96});
		eventBarSprite[i]->SetTextureRect(0,0,256,96);
		eventBarSprite[i]->SetPosition({640,48 + (float)i*624});
		eventBarSprite[i]->SetColor({1,1,1,0.9f});
	}

#pragma endregion
}

void Select::Update()
{
#pragma region オブジェクト更新
	//天球 更新
	skydomeRot += {0,XMConvertToRadians(skydomeRotSp),0};
	skydomeObject->SetRotation(skydomeRot);
	skydomeObject->Update();
	skydome->Update();
	//地面 更新
	ground->Update();
#pragma endregion

#pragma region 汎用機能 更新
	this->camera->Update();
#pragma endregion

#pragma region スプライト更新

	//矢印
	ArrowUpdate();

	//ナンバー
	AreaNumberUpdate();

	//シーン遷移
	SceneChange();

#pragma endregion

#ifdef _DEBUG
	if(input->Trigger(DIK_F1))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::DEBUGAREA;
	}
	else if(input->Trigger(DIK_F2))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::DEBUGAREA02;
	}
	else if(input->Trigger(DIK_1))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::TUTORIAL01;
	}
	else if(input->Trigger(DIK_2))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::TUTORIAL02;
	}
	else if(input->Trigger(DIK_3))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::TUTORIAL03;
	}
	else if(input->Trigger(DIK_4))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::TUTORIAL04;
	}
	else if(input->Trigger(DIK_5))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::TUTORIAL05;
	}
	else if(input->Trigger(DIK_6))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::AREA01;
	}
	else if(input->Trigger(DIK_7))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::AREA02;
	}
	else if(input->Trigger(DIK_8))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::AREA03;
	}
	else if(input->Trigger(DIK_9))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::AREA04;
	}

	this->debugText->Printf(0,64,1.f,"Select Area");
	this->debugText->Printf(0,96,1.f,"AreaNumber : %d", areaNumber);
	this->debugText->Printf(0,128,1.f,"SceneChange : %d", sceneChange);
	this->debugText->Printf(0,182,1.f,"AreaPos 1:%d, 2:%d, 3:%d, 4:%d, 5:%d, 6:%d, 7:%d, 8:%d, 9:%d",
		areaPosIndices[0],areaPosIndices[1],areaPosIndices[2],areaPosIndices[3],areaPosIndices[4],
		areaPosIndices[5],areaPosIndices[6],areaPosIndices[7],areaPosIndices[8]);

#endif // _DEBUG
}

void Select::Draw()
{
	//DirectXCommon前処理
	this->dxCommon->BeginDraw();

#pragma region オブジェクト描画
	//天球 描画
	skydome->Draw(dxCommon->GetCommandList());
	//地面 描画
	ground->Draw(dxCommon->GetCommandList());
#pragma endregion

#pragma region スプライト描画
	//スプライト
	Sprite::SetPipelineState(this->dxCommon->GetCommandList());

	//矢印
	for(int i = 0; i < 2; i++)
	{
		arrow[i]->Draw(dxCommon->GetCommandList());
	}

	//ナンバー
	for(int i = 0; i < 9; i++)
	{
		area[i]->Draw(dxCommon->GetCommandList());
	}

	//イベント演出
	for(int i = 0; i < 2; i++){
		eventBarSprite[i]->Draw(this->dxCommon->GetCommandList());
	}
#pragma endregion

#ifdef _DEBUG

	this->debugText->DrawAll(this->dxCommon->GetCommandList());

#endif // _DEBUG

	//DirectXCommon前処理
	this->dxCommon->EndDraw();
}

void Select::Finalize()
{
}

void Select::ArrowInitialize()
{
	//矢印
	for(int i = 0; i < 2; i++)
	{
		arrow[i] = make_unique<Sprite>();
		arrow[i]->Initialize(1);
		arrow[i]->SetAnchorpoint({0.5f,0.5f});
		arrow[i]->SetPosition({96 + (float)i*1088, 360});
		arrow[i]->SetSize({128,128});
		arrow[i]->SetTextureRect(0,0,256,256);
		arrow[i]->SetColor({1,1,1,1});
	}
	arrow[0]->SetIsFlipX(true);
}

void Select::ArrowUpdate()
{
	//矢印
	if(!IsArrowAnim)
	{
		//矢印0
		arrow0Pos[0] = {arrow[0]->GetPosition().x, arrow[0]->GetPosition().y};
		arrow0Pos[1] = {arrow[0]->GetPosition().x + 20, arrow[0]->GetPosition().y};
		arrow0Pos[2] = {arrow[0]->GetPosition().x, arrow[0]->GetPosition().y};
		arrowTempPos[0] = arrow0Pos[0];

		//矢印1
		arrow1Pos[0] = {arrow[1]->GetPosition().x, arrow[1]->GetPosition().y};
		arrow1Pos[1] = {arrow[1]->GetPosition().x - 20, arrow[1]->GetPosition().y};
		arrow1Pos[2] = {arrow[1]->GetPosition().x, arrow[1]->GetPosition().y};
		arrowTempPos[1] = arrow1Pos[0];

		IsArrowAnim = true;
	}
	else if(IsArrowAnim)
	{
		//時間計算
		arrowAnimFrame += 1.f / 60.f;
		float a = (1 - arrowAnimFrame);

		//矢印0
		arrowTempPos[0].x = a*a*arrow0Pos[0].x + 2.f*a*arrowAnimFrame*arrow0Pos[1].x + arrowAnimFrame*arrowAnimFrame*arrow0Pos[2].x;
		arrowTempPos[0].y = a*a*arrow0Pos[0].y + 2.f*a*arrowAnimFrame*arrow0Pos[1].y + arrowAnimFrame*arrowAnimFrame*arrow0Pos[2].y;

		//矢印1
		arrowTempPos[1].x = a*a*arrow1Pos[0].x + 2.f*a*arrowAnimFrame*arrow1Pos[1].x + arrowAnimFrame*arrowAnimFrame*arrow1Pos[2].x;
		arrowTempPos[1].y = a*a*arrow1Pos[0].y + 2.f*a*arrowAnimFrame*arrow1Pos[1].y + arrowAnimFrame*arrowAnimFrame*arrow1Pos[2].y;

		//ループ
		if(arrowAnimFrame >= 1)
		{
			arrowTempPos[0] = arrow0Pos[2];
			arrowTempPos[1] = arrow1Pos[2];
			arrowAnimFrame = 0.f;
			IsArrowAnim = false;
		}
	}
	arrow[0]->SetPosition(arrowTempPos[0]);
	arrow[1]->SetPosition(arrowTempPos[1]);
}

void Select::AreaNumberInitialize()
{
	for(int i = 0; i < 9; i++)
	{
		Select::areaPosIndices[i] = i;

		area[i] = make_unique<Sprite>();
		area[i]->Initialize(i + 2);
		area[i]->SetAnchorpoint({0.5f,0.5f});
		area[i]->SetPosition({640+(float)(Select::areaPosIndices[i])*640, 360});
		area[i]->SetSize({384, 384});
		area[i]->SetTextureRect(0,0,256,256);
		area[i]->SetColor({1,1,1,1});
	}
}

void Select::AreaNumberUpdate()
{
	if(input->Trigger(DIK_D) || input->Trigger(DIK_RIGHT))
	{
		if(areaNumber == 9) return;

		//座標インデックス変更
		for(int i = 0; i < 9; i++){
			Select::areaPosIndices[i]--;
		}
		areaNumber++;
	}
	else if(input->Trigger(DIK_A) || input->Trigger(DIK_LEFT))
	{
		if(areaNumber == 1) return;

		//座標インデックス変更
		for(int i = 0; i < 9; i++){
			Select::areaPosIndices[i]++;
		}
		areaNumber--;
	}

	//座標更新
	for(int i = 0; i < 9; i++){
		area[i]->SetPosition({640+(float)(Select::areaPosIndices[i])*640, 360});
	}
}

void Select::SceneChange()
{
	if(input->Trigger(DIK_SPACE) || input->Trigger(DIK_RETURN) ||
		input->Trigger(DIK_W)|| input->Trigger(DIK_UP))
	{
		if(areaNumber == 1){
			sceneChange = true;
			sceneChangeNumber = SCENE::TUTORIAL01;
		}
		else if(areaNumber == 2){
			sceneChange = true;
			sceneChangeNumber = SCENE::TUTORIAL02;
		}
		else if(areaNumber == 3){
			sceneChange = true;
			sceneChangeNumber = SCENE::TUTORIAL03;
		}
		else if(areaNumber == 4){
			sceneChange = true;
			sceneChangeNumber = SCENE::TUTORIAL04;
		}
		else if(areaNumber == 5){
			sceneChange = true;
			sceneChangeNumber = SCENE::TUTORIAL05;
		}
		else if(areaNumber == 6){
			sceneChange = true;
			sceneChangeNumber = SCENE::AREA01;
		}
		else if(areaNumber == 7){
			sceneChange = true;
			sceneChangeNumber = SCENE::AREA02;
		}
		else if(areaNumber == 8){
			sceneChange = true;
			sceneChangeNumber = SCENE::AREA03;
		}
		else if(areaNumber == 9){
			sceneChange = true;
			sceneChangeNumber = SCENE::AREA04;
		}
	}
}