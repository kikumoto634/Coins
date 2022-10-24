#include "Title.h"

using namespace std;

void Title::AppInitialize(WinApp *winapp, DirectXCommon *dxCommon, Input *input, TextureManager *textureManager, GeometryManager *geometryModel, Camera *camera, DebugText *debugText, SoundManager *soundManager)
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

void Title::Initialize()
{
#pragma region �I�u�W�F�N�g������
	/// <summary>
	/// �V��
	/// </summary>
	skydomeModel = make_unique<Model>();
	skydomeModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("skydome"));
	skydomeObject = make_unique<Object3D>();
	skydome = make_unique<Skydome>();
	skydome->Initialize(skydomeModel.get(), skydomeObject.get());
	skydomeRot = {0,0,0};

	/// <summary>
	/// �n��
	/// </summary>
	groundModel = make_unique<Model>();
	groundModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("Area01"));
	groundObject = make_unique<Object3D>();
	groundObject->SetScale({0.3f,0.3f,0.3f});
	ground = make_unique<Ground>();
	ground->Initialize(groundModel.get(), groundObject.get(), 0.f, {0,-20,0});

	camera->SetEye({0,100,-200});
#pragma endregion

#pragma region �X�v���C�g������

	this->textureManager->LoadTexture(1,L"Resources/Title/Title.png");

	logPosIni = {640,160};
	logPos = logPosIni;
	logPosRail[0] = logPosIni;
	logPosRail[1] = {640, 170};
	logPosRail[2] = logPosIni;
	logPosRail[3] = {640, 150};
	logPosRail[4] = logPosIni;

	titleLog = make_unique<Sprite>();
	titleLog->Initialize(1);
	titleLog->SetAnchorpoint({0.5f, 0.5f});
	titleLog->SetSize({640,256});
	titleLog->SetTextureRect(0,0,640,256);
	titleLog->SetPosition(logPos);

#pragma endregion
}

void Title::Update()
{
#pragma region �X�v���C�g�X�V

	titleLog->SetPosition(logPos);

	spiteMoveTime += 1.f/(60*spritemoveSecond);
	float a = (1-spiteMoveTime);

	logPos.y = a*a*a*a*logPosRail[0].y + 4.f*a*a*a*spiteMoveTime*logPosRail[1].y + 4.f*a*a*spiteMoveTime*spiteMoveTime*logPosRail[2].y + 4.f*a*spiteMoveTime*spiteMoveTime*spiteMoveTime*logPosRail[3].y + spiteMoveTime*spiteMoveTime*spiteMoveTime*spiteMoveTime*logPosRail[4].y;

	if(spiteMoveTime >= 1.f){
		spiteMoveTime = 0.f;
		logPos = logPosIni;
	}

#pragma endregion

#pragma region �I�u�W�F�N�g�X�V
	//�V�� �X�V
	skydomeRot += {0,XMConvertToRadians(skydomeRotSp),0};
	skydomeObject->SetRotation(skydomeRot);
	skydomeObject->Update();
	skydome->Update();
	//�n�� �X�V
	ground->Update();
#pragma endregion

#pragma region �ėp�@�\ �X�V
	this->camera->Update();
#pragma endregion

	//�V�[���J��
	if(input->Trigger(DIK_SPACE) || input->Trigger(DIK_RETURN)){
		sceneChange = true;
		sceneChangeNumber = SCENE::SELECT;
	}


#ifdef _DEBUG

	if(input->Trigger(DIK_SPACE))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::SELECT;
	}

	this->debugText->Printf(0,64,1.f,"Title Area");

#endif // _DEBUG
}

void Title::Draw()
{
	//DirectXCommon�O����
	this->dxCommon->BeginDraw();

#pragma region �I�u�W�F�N�g�`��
	//�V�� �`��
	skydome->Draw(dxCommon->GetCommandList());
	//�n�� �`��
	ground->Draw(dxCommon->GetCommandList());
#pragma endregion

#pragma region �X�v���C�g�`��
	//�X�v���C�g
	Sprite::SetPipelineState(this->dxCommon->GetCommandList());

	//�^�C�g��
	titleLog->Draw(this->dxCommon->GetCommandList());
#pragma endregion

#ifdef _DEBUG

	this->debugText->DrawAll(this->dxCommon->GetCommandList());

#endif // _DEBUG

	//DirectXCommon�O����
	this->dxCommon->EndDraw();
}

void Title::Finalize()
{
}
