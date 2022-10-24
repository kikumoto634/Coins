#pragma once
#include "GameBase.h"
#include "Skydome.h"
#include "Ground.h"

class Title : public GameBase
{
//テンプレート
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

	//スプライト
	unique_ptr<Sprite> titleLog;

	Vector2 logPosIni{};
	Vector2 logPos{};
	Vector2 logPosRail[5] = {};
	float spritemoveSecond = 5.f;
	float spiteMoveTime = 0.f;

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

