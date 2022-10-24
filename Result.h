#pragma once
#include "Sprite.h"

/// <summary>
/// ���U���g����
/// </summary>
class Result
{
private:
	enum class State
	{
		RESULT,		//���U���g
		RESTART,	//���X�^�[�g
	};

private:
	const float second = 60.f;

	//�S�[�����o�t�F�[�h�C���b��
	const float GoalSpriteFadeInTime = 0.5f * second;
	//�S�[�����o�R�C��
	const float GoalCoinSpriteEaseTime = 0.5f * second;
	//�S�[�����o�R�C���T�C�Y
	const float GoalCoinMaxSize = 140.f;

public:
	//�X�v���C�g������
	void BackGroundSpriteInitialize(Sprite* background);
	void CoinSpriteInitialize(Sprite* coin, int indices);
	void ResultTextInitialize(Sprite* result);
	void ClearTextInitialie(Sprite* clear);
	void RestartTextInitialize(Sprite* restart);

	//�X�V
	void GoalUpdate();
	//�`��
	void GoalDraw(ID3D12GraphicsCommandList* commandList);

	bool GetIsGoal() {return IsGoal;}
	bool GetIsDead() {return IsDead;}

	bool GetIsSceneChange()	{return IsSceneChange;}

	void SetCoinNum(int num)	{coins = num;}
	void SetIsGoal(bool isGoal)	{IsGoal = isGoal;}
	void SetIsDead(bool isDead) {IsDead = isDead;}

private:
	//���U���g��ʂ̓��ߗ�
	void GoalSpriteAlpha();
	//�R�C�����擾�w�i
	void GoalCoinBackUpdate();
	//�R�C���擾�w�i
	void GoalCoinUpdate();

	//���S���ɃR�C�����������o
	void CoinDelete();

public:

#pragma region �X�v���C�g�֘A
	Sprite* backGround;
	Sprite* RESULTtext;
	Sprite* CLEARtext;
	Sprite* RESTARTtext;

	Sprite* coinSprite[3];
#pragma endregion

	State state = State::RESULT;

	//�R�C������
	int coins = 0;

	//Goal
	bool IsGoal = false;
	bool IsDead = false;

	//�V�[���J�ډ\
	bool IsSceneChange = false;

	//���ߗ��ύX����
	float alphaTIme = 0.f;
	//���ߗ�
	float GoalAlpha = 0.f;

	//�V�F�C�N
	const int Shake = 5;
	const float ShakeBreakTime = 2.5f * second;
	float shakeTime = 0.f;

	//�R�C���T�C�Y�ύX����
	float coinSizeTime = 0.f;
	//�S�[�����o�R�C���T�C�Y
	float GoalCoinSize = 0.f;
};

