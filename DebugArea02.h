#pragma once
#include "GameBase.h"
#include "Skydome.h"
#include "Ground.h"

class DebugArea02 : public GameBase
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
	unique_ptr<Model> model;
	unique_ptr<Object3D> object;
	Vector3 position = {0,0,0};
	Vector3 rotation = {0,0,0};

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

