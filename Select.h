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
	//矢印
	void ArrowInitialize();
	void ArrowUpdate();

	//ナンバー
	void AreaNumberInitialize();
	void AreaNumberUpdate();

	//シーン遷移
	void SceneChange();

private:

	//矢印
	unique_ptr<Sprite> arrow[2];
	Vector2 arrow0Pos[3];
	Vector2 arrow1Pos[3];
	Vector2 arrowTempPos[2];
	bool IsArrowAnim = false;
	float arrowAnimFrame = 0.f;

	//エリア
	unique_ptr<Sprite> area[9];
	int areaNumber;
	int areaPosIndices[9];

	//イベント演出
	unique_ptr<Sprite> eventBarSprite[2];

	//天球
	unique_ptr<Model> skydomeModel;
	unique_ptr<Object3D> skydomeObject;
	unique_ptr<Skydome> skydome;
	Vector3 skydomeRot{};
	float skydomeRotSp = 1.f;

	///地面
	unique_ptr<Model> groundModel;
	unique_ptr<Object3D> groundObject;
	unique_ptr<Ground> ground;

#pragma region ゲームベース

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

