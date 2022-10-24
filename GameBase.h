#pragma once

//Windows
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "GeometryManager.h"
#include "GeometryObject3D.h"
#include "Camera.h"
#include "DebugText.h"
#include "SoundManager.h"
#include "Object3D.h"
#include "FbxLoader.h"

#include "Scenes.h"

/// <summary>
/// ゲーム共通基盤　(フレームワーク)
/// </summary>
class GameBase
{
/// <summary>
/// メンバ関数
/// </summary>
public:

	/// 実行
	/// </summary>
	virtual void Run();

	void GameBaseInitialize();

	virtual void AppInitialize(WinApp* winapp, DirectXCommon* dxCommon, Input* input, TextureManager* textureManager, 
		GeometryManager* geometryModel, Camera* camera, DebugText* debugText, SoundManager* soundManager) = 0;

	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 解放
	/// </summary>
	virtual void Finalize();

	virtual bool GetSceneChange() = 0;
	virtual SCENE GetSceneChangeNumber() = 0;

protected:
	//終了フラグ
	bool endFlag = false;

	//シーン遷移
	bool sceneChange = false;
	SCENE sceneChangeNumber = SCENE::NONE;

	//WindowsAPIオブジェクト
	std::unique_ptr<WinApp> winApp;

	//DirectXオブジェクト
	std::unique_ptr<DirectXCommon> dxCommon;

	//入力
	std::unique_ptr<Input> input;

	//テクスチャマネージャー
	std::unique_ptr<TextureManager> textureManager;

	//モデル
	std::unique_ptr<GeometryManager> geometryModel;

	//カメラ
	std::unique_ptr<Camera> camera;

	//デバック
	std::unique_ptr<DebugText> debugText;

	//サウンドマネージャー
	std::unique_ptr<SoundManager> soundManager;
};

