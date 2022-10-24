#pragma once
#include "GameBase.h"
#include "Player.h"
#include "Coin.h"
#include "Obstacle.h"
#include "slideWall.h"
#include "Ground.h"
#include "Skydome.h"
#include "Goal.h"

#include "CollisionManager.h"
#include "Result.h"

#include <list>
#include <sstream>

class DebugArea : public GameBase
{
//テンプレート
public:
	template <class T> using unique_ptr = std::unique_ptr<T>;
	template <class T> using list = std::list<T>;

//定数
private:
	const float ObjectsVelocity = 2.5f;

//override
public:
#pragma region 継承関数(汎用機能)
	/// <summary>
	/// アプリケーション初期化
	/// </summary>
	/// <param name="winapp">Windows</param>
	/// <param name="dxCommon">コモン</param>
	/// <param name="input">入力</param>
	/// <param name="textureManager">テクスチャ</param>
	/// <param name="geometryModel">ジオメトリ</param>
	/// <param name="camera">カメラ</param>
	/// <param name="debugText">デバックテキスト</param>
	/// <param name="soundManager">サウンド</param>
	void AppInitialize(WinApp* winapp, DirectXCommon* dxCommon, Input* input, TextureManager* textureManager, 
		GeometryManager* geometryModel, Camera* camera, DebugText* debugText, SoundManager* soundManager) override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 解法
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// scene遷移フラグ
	/// </summary>
	/// <returns>bool</returns>
	bool GetSceneChange() override	{return sceneChange;}
	/// <summary>
	/// 遷移先
	/// </summary>
	/// <returns>SCENE</returns>
	SCENE GetSceneChangeNumber() override	{return sceneChangeNumber;}
#pragma endregion

//メンバ関数
private:
	
	//コインPOP
	void LoadCoinPopData();
	void UpdateCoinPopCommands();
	void CoinPop(Vector3 pos);

	//壁POP
	void LoadWallPopData();
	void UpdateWallPopCommands();
	void WallPop(Vector3 pos);

	//しゃがみ壁Wall
	void LoadSlideWallPopData();
	void UpdateSlideWallPopCommands();
	void SlideWallPop(Vector3 pos);

	//ゴール処理
	void GoalUpdate();
	void DeadUpdate();
	void ObjectVelocityStop();

	//衝突判定と応答
	void CheckAllCollision();
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	bool CheckCollisionDetail(Collider* colliderA, Collider* colliderB);

//メンバ変数
private:

	//プレイヤー
	unique_ptr<Model> playerModel;
	unique_ptr<Object3D> playerObject;
	unique_ptr<Player> player;

	//HP
	unique_ptr<Sprite> HpSprite[3];
	//Bottom
	unique_ptr<Sprite> ButtonUp;
	unique_ptr<Sprite> ButtonDown;
	unique_ptr<Sprite> ButtonLeft;
	unique_ptr<Sprite> ButtonRight;
	unique_ptr<Sprite> ButtonW;
	unique_ptr<Sprite> ButtonS;

	//コイン
	list<unique_ptr<Model>> coinModel;
	list<unique_ptr<Object3D>> coinObject;
	list<unique_ptr<Coin>> coin;
	std::stringstream coinPopCommands;

	//スコア加算UI
	unique_ptr<Sprite> coinIncrement;
	
	//ダメージエフェクト
	unique_ptr<Sprite> damageEffect;

	//壁
	list<unique_ptr<Model>> wallModel;
	list<unique_ptr<Object3D>> wallObject;
	list<unique_ptr<Obstacle>> wall;
	std::stringstream wallPopCommands;

	//しゃがみ壁
	list<std::unique_ptr<Model>> slideWallModel;
	list<std::unique_ptr<Object3D>> slideWallObject;
	list<std::unique_ptr<slideWall>> lslideWall;
	std::stringstream slideWallPopCommands;

	///天球
	unique_ptr<Model> skydomeModel;
	unique_ptr<Object3D> skydomeObject;
	unique_ptr<Skydome> skydome;
	Vector3 skydomeRot{};
	float skydomeRotSp = 1.f;

	///地面
	unique_ptr<Model> groundModel;
	unique_ptr<Object3D> groundObject;
	unique_ptr<Ground> ground;

	//ゴール
	unique_ptr<Model> goalModel;
	unique_ptr<Object3D> goalObject;
	unique_ptr<Goal> goal;

	//衝突判定
	unique_ptr<CollisionManager> collisionManager;

	//リザルト
	unique_ptr<Result> result;
	std::unique_ptr<Sprite> backGround;
	std::unique_ptr<Sprite> RESULTtext;
	std::unique_ptr<Sprite> CLEARtext;
	std::unique_ptr<Sprite> RESTARTtext;
	std::unique_ptr<Sprite> coinSprite[3];

//protected
private:
#pragma region 継承変数(汎用機能)
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

