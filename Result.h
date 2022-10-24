#pragma once
#include "Sprite.h"

/// <summary>
/// リザルト処理
/// </summary>
class Result
{
private:
	enum class State
	{
		RESULT,		//リザルト
		RESTART,	//リスタート
	};

private:
	const float second = 60.f;

	//ゴール演出フェードイン秒数
	const float GoalSpriteFadeInTime = 0.5f * second;
	//ゴール演出コイン
	const float GoalCoinSpriteEaseTime = 0.5f * second;
	//ゴール演出コインサイズ
	const float GoalCoinMaxSize = 140.f;

public:
	//スプライト初期化
	void BackGroundSpriteInitialize(Sprite* background);
	void CoinSpriteInitialize(Sprite* coin, int indices);
	void ResultTextInitialize(Sprite* result);
	void ClearTextInitialie(Sprite* clear);
	void RestartTextInitialize(Sprite* restart);

	//更新
	void GoalUpdate();
	//描画
	void GoalDraw(ID3D12GraphicsCommandList* commandList);

	bool GetIsGoal() {return IsGoal;}
	bool GetIsDead() {return IsDead;}

	bool GetIsSceneChange()	{return IsSceneChange;}

	void SetCoinNum(int num)	{coins = num;}
	void SetIsGoal(bool isGoal)	{IsGoal = isGoal;}
	void SetIsDead(bool isDead) {IsDead = isDead;}

private:
	//リザルト画面の透過率
	void GoalSpriteAlpha();
	//コイン未取得背景
	void GoalCoinBackUpdate();
	//コイン取得背景
	void GoalCoinUpdate();

	//死亡時にコインを消す演出
	void CoinDelete();

public:

#pragma region スプライト関連
	Sprite* backGround;
	Sprite* RESULTtext;
	Sprite* CLEARtext;
	Sprite* RESTARTtext;

	Sprite* coinSprite[3];
#pragma endregion

	State state = State::RESULT;

	//コイン枚数
	int coins = 0;

	//Goal
	bool IsGoal = false;
	bool IsDead = false;

	//シーン遷移可能
	bool IsSceneChange = false;

	//透過率変更時間
	float alphaTIme = 0.f;
	//透過率
	float GoalAlpha = 0.f;

	//シェイク
	const int Shake = 5;
	const float ShakeBreakTime = 2.5f * second;
	float shakeTime = 0.f;

	//コインサイズ変更時間
	float coinSizeTime = 0.f;
	//ゴール演出コインサイズ
	float GoalCoinSize = 0.f;
};

