#include "Result.h"


void Result::BackGroundSpriteInitialize(Sprite *background)
{
	this->backGround = background;

	//backGround->Initialize(1);
	backGround->SetPosition({240,135});	//320,180
	backGround->SetSize({800,450});	//640,360
	backGround->SetColor({1,1,1,GoalAlpha});
}

void Result::CoinSpriteInitialize(Sprite *coin, int indices)
{
	this->coinSprite[indices] = coin;

	//coinSprite[indices]->Initialize(3);
	coinSprite[indices]->SetAnchorpoint({0.5f,0.5f});
	coinSprite[indices]->SetPosition({440 + (float)indices * 200, 450});
	coinSprite[indices]->SetSize({GoalCoinSize,GoalCoinSize});
	coinSprite[indices]->SetTextureRect(0,0,512,512);
	coinSprite[indices]->SetColor({1,1,1,GoalAlpha});
}

void Result::ResultTextInitialize(Sprite *result)
{
	this->RESULTtext = result;

	//RESULTtext->Initialize(5);
	RESULTtext->SetPosition({490,150});
	RESULTtext->SetSize({300,80});
	RESULTtext->SetTextureRect(0,0,286,74);
	RESULTtext->SetColor({1,1,1,GoalAlpha});
}

void Result::ClearTextInitialie(Sprite *clear)
{
	this->CLEARtext = clear;

	//CLEARtext->Initialize(2);
	CLEARtext->SetPosition({510,260});
	CLEARtext->SetSize({260,70});
	CLEARtext->SetTextureRect(0,0,244,71);
	CLEARtext->SetColor({1,1,1,GoalAlpha});
}

void Result::RestartTextInitialize(Sprite *restart)
{
	this->RESTARTtext = restart;

	//RESTARTtext->Initialize(2);
	RESTARTtext->SetPosition({510,260});
	RESTARTtext->SetSize({260,70});
	RESTARTtext->SetTextureRect(0,0,244,71);
	RESTARTtext->SetColor({1,1,1,GoalAlpha});
}


void Result::GoalSpriteAlpha()
{
	//背景
	alphaTIme += 1.f/GoalSpriteFadeInTime;
	//easeOut
	float easeT = 1.f - powf(1.f-alphaTIme, 3.f);
	GoalAlpha = (1.f-easeT)*0.f + easeT*1.f;

	if(GoalAlpha >= 1.f) return ;

	backGround->SetColor({1,1,1,GoalAlpha});
	RESULTtext->SetColor({1,1,1,GoalAlpha});
	CLEARtext->SetColor({1,1,1,GoalAlpha});
	for(int i = 0; i < 3; i++)
	{
		coinSprite[i]->SetColor({1,1,1,GoalAlpha});
	}
}

void Result::GoalCoinBackUpdate()
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.f;
	float easeT = 1.f + c3 * powf(coinSizeTime - 1.f, 3.f) + c1 * powf(coinSizeTime - 1.f, 2.f);

	GoalCoinSize = (1.f - easeT)*0.f + easeT*GoalCoinMaxSize;
	for(int i = 0; i < 3; i++)
	{
		coinSprite[i]->SetSize({GoalCoinSize, GoalCoinSize});
	}

	if(easeT >= 1.1f) return;
	coinSizeTime += 1.f/GoalCoinSpriteEaseTime;
}

void Result::GoalCoinUpdate()
{
	for(int i = 0; i < coins; i++)
	{
		coinSprite[i]->Initialize(4);
		coinSprite[i]->SetAnchorpoint({0.5f,0.5f});
		coinSprite[i]->SetPosition({440 + (float)i * 200, 450});
		coinSprite[i]->SetSize({GoalCoinSize,GoalCoinSize});
		coinSprite[i]->SetTextureRect(0,0,512,512);
	}

	//シーン遷移可能
	IsSceneChange = true;
}

void Result::CoinDelete()
{
	if(coins == 0) return;

	//終了
	if(shakeTime >= ShakeBreakTime)
	{
		for(int i = 0; i < coins; i++)
		{
			coinSprite[i]->Initialize(3);
			coinSprite[i]->SetAnchorpoint({0.5f,0.5f});
			coinSprite[i]->SetPosition({440 + (float)i * 200, 450});
			coinSprite[i]->SetSize({GoalCoinSize,GoalCoinSize});
			coinSprite[i]->SetTextureRect(0,0,512,512);

			shakeTime = ShakeBreakTime;
		}

		//シーン遷移可能
		IsSceneChange = true;

		return;
	}

	//シェイク
	for(int i = 0; i < coins; i++)
	{
		Vector2 randPos = {float(rand()%Shake), float(rand()%Shake)};
		coinSprite[i]->SetPosition(coinSprite[i]->GetPosition() + randPos);
	}

	//タイム
	shakeTime += 1.f;
}

void Result::GoalUpdate()
{
	if(!IsGoal && !IsDead) return;

	//スプライトアルファ
	GoalSpriteAlpha();
	//コイン背面
	GoalCoinBackUpdate();
	//コイン前面
	GoalCoinUpdate();

	if(IsDead)
	{
		CoinDelete();
	}
}

void Result::GoalDraw(ID3D12GraphicsCommandList* commandList)
{
	if(!IsGoal && !IsDead) return ;

	backGround->Draw(commandList);
	RESULTtext->Draw(commandList);;

	for(int i = 0; i < 3; i++){
		coinSprite[i]->Draw(commandList);
	}
}