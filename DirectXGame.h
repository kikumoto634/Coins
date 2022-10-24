#pragma once
#include "GameBase.h"

#include <list>
#include <sstream>

class DirectXGame : public GameBase
{
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
	void SceneChange();

private:
	//シーン
	std::unique_ptr<GameBase> scene;
	SCENE sceneNumber;


	bool IsGameStart = false;
	

	//GameBase
	WinApp* winApp;
	DirectXCommon* dxCommon;
	Input* input;
	TextureManager* textureManager;
	GeometryManager* geometryModel;
	Camera* camera;
	DebugText* debugText;
	SoundManager* soundManager;
};

