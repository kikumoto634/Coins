#pragma once
#include "GameBase.h"
#include "Skydome.h"
#include "Ground.h"

class Select : public GameBase
{
public:
	template <class T> using unique_ptr = std::unique_ptr<T>;

public:
	void AppInitialize(WinApp* winapp, DirectXCommon* dxCommon, Input* input, TextureManager* textureManager, 
		GeometryManager* geometryModel, Camera* camera, DebugText* debugText, SoundManager* soundManager) override;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	bool GetSceneChange() override	{return sceneChange;}
	SCENE GetSceneChangeNumber() override	{return sceneChangeNumber;}

private:
	//���
	void ArrowInitialize();
	void ArrowUpdate();

	//�i���o�[
	void AreaNumberInitialize();
	void AreaNumberUpdate();

	//�V�[���J��
	void SceneChange();

private:

	//���
	unique_ptr<Sprite> arrow[2];
	Vector2 arrow0Pos[3];
	Vector2 arrow1Pos[3];
	Vector2 arrowTempPos[2];
	bool IsArrowAnim = false;
	float arrowAnimFrame = 0.f;

	//�G���A
	unique_ptr<Sprite> area[9];
	int areaNumber;
	int areaPosIndices[9];

	//�C�x���g���o
	unique_ptr<Sprite> eventBarSprite[2];

	//�V��
	unique_ptr<Model> skydomeModel;
	unique_ptr<Object3D> skydomeObject;
	unique_ptr<Skydome> skydome;
	Vector3 skydomeRot{};
	float skydomeRotSp = 1.f;

	///�n��
	unique_ptr<Model> groundModel;
	unique_ptr<Object3D> groundObject;
	unique_ptr<Ground> ground;

#pragma region �Q�[���x�[�X

	WinApp* winApp;
	DirectXCommon* dxCommon;
	Input* input;
	TextureManager* textureManager;
	GeometryManager* geometryModel;
	Camera* camera;
	DebugText* debugText;
	SoundManager* soundManager;

#pragma endregion
};

