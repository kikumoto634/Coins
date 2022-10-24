#include "DebugArea02.h"

using namespace std;

void DebugArea02::AppInitialize(WinApp *winapp, DirectXCommon *dxCommon, Input *input, TextureManager *textureManager, GeometryManager *geometryModel, Camera *camera, DebugText *debugText, SoundManager *soundManager)
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

void DebugArea02::Initialize()
{
#pragma region �I�u�W�F�N�g������
	this->camera->SetTarget({0,0,0});
	/*eye = {0, distance, -distance};
	target = {0, 0, upRot};
	up = {0, 1, 0};*/

	model = make_unique<Model>();
	model = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("Player"));
	object = make_unique<Object3D>();
	object->Initialize();
	object->SetModel(model.get());
	object->SetPosition(position);
	object->SetRotation(rotation);
	object->SetScale({1,1,1});

#pragma endregion

#pragma region �X�v���C�g������


#pragma endregion
}

void DebugArea02::Update()
{
#pragma region �I�u�W�F�N�g�X�V

	//�I�u�W�F�N�g��]
	if(input->Push(DIK_LEFT)){
		rotation.y += 1.f;
	}
	else if(input->Push(DIK_RIGHT)){
		rotation.y -= 1.f;
	}

	//�J������]
	Vector3 eye = camera->GetEye();
	if(input->Push(DIK_A)){
		eye.x -= 2.f;
		camera->SetEye(eye);
	}
	else if(input->Push(DIK_D)){
		eye.x += 2.f;
		camera->SetEye(eye);
	}

	object->SetRotation(rotation);
	object->Update();

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

	this->debugText->Printf(0,64,1.f,"DebugArea02 Area");

#endif // _DEBUG
}

void DebugArea02::Draw()
{
	//DirectXCommon�O����
	this->dxCommon->BeginDraw();

#pragma region �I�u�W�F�N�g�`��
	object->Draw(dxCommon->GetCommandList());
#pragma endregion

#pragma region �X�v���C�g�`��
	//�X�v���C�g
	Sprite::SetPipelineState(this->dxCommon->GetCommandList());


#pragma endregion

#ifdef _DEBUG

	this->debugText->DrawAll(this->dxCommon->GetCommandList());

#endif // _DEBUG

	//DirectXCommon�O����
	this->dxCommon->EndDraw();
}

void DebugArea02::Finalize()
{
}
