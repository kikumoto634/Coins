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
//�e���v���[�g
public:
	template <class T> using unique_ptr = std::unique_ptr<T>;
	template <class T> using list = std::list<T>;

//�萔
private:
	const float ObjectsVelocity = 2.5f;

//override
public:
#pragma region �p���֐�(�ėp�@�\)
	/// <summary>
	/// �A�v���P�[�V����������
	/// </summary>
	/// <param name="winapp">Windows</param>
	/// <param name="dxCommon">�R����</param>
	/// <param name="input">����</param>
	/// <param name="textureManager">�e�N�X�`��</param>
	/// <param name="geometryModel">�W�I���g��</param>
	/// <param name="camera">�J����</param>
	/// <param name="debugText">�f�o�b�N�e�L�X�g</param>
	/// <param name="soundManager">�T�E���h</param>
	void AppInitialize(WinApp* winapp, DirectXCommon* dxCommon, Input* input, TextureManager* textureManager, 
		GeometryManager* geometryModel, Camera* camera, DebugText* debugText, SoundManager* soundManager) override;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ��@
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// scene�J�ڃt���O
	/// </summary>
	/// <returns>bool</returns>
	bool GetSceneChange() override	{return sceneChange;}
	/// <summary>
	/// �J�ڐ�
	/// </summary>
	/// <returns>SCENE</returns>
	SCENE GetSceneChangeNumber() override	{return sceneChangeNumber;}
#pragma endregion

//�����o�֐�
private:
	
	//�R�C��POP
	void LoadCoinPopData();
	void UpdateCoinPopCommands();
	void CoinPop(Vector3 pos);

	//��POP
	void LoadWallPopData();
	void UpdateWallPopCommands();
	void WallPop(Vector3 pos);

	//���Ⴊ�ݕ�Wall
	void LoadSlideWallPopData();
	void UpdateSlideWallPopCommands();
	void SlideWallPop(Vector3 pos);

	//�S�[������
	void GoalUpdate();
	void DeadUpdate();
	void ObjectVelocityStop();

	//�Փ˔���Ɖ���
	void CheckAllCollision();
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	bool CheckCollisionDetail(Collider* colliderA, Collider* colliderB);

//�����o�ϐ�
private:

	//�v���C���[
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

	//�R�C��
	list<unique_ptr<Model>> coinModel;
	list<unique_ptr<Object3D>> coinObject;
	list<unique_ptr<Coin>> coin;
	std::stringstream coinPopCommands;

	//�X�R�A���ZUI
	unique_ptr<Sprite> coinIncrement;
	
	//�_���[�W�G�t�F�N�g
	unique_ptr<Sprite> damageEffect;

	//��
	list<unique_ptr<Model>> wallModel;
	list<unique_ptr<Object3D>> wallObject;
	list<unique_ptr<Obstacle>> wall;
	std::stringstream wallPopCommands;

	//���Ⴊ�ݕ�
	list<std::unique_ptr<Model>> slideWallModel;
	list<std::unique_ptr<Object3D>> slideWallObject;
	list<std::unique_ptr<slideWall>> lslideWall;
	std::stringstream slideWallPopCommands;

	///�V��
	unique_ptr<Model> skydomeModel;
	unique_ptr<Object3D> skydomeObject;
	unique_ptr<Skydome> skydome;
	Vector3 skydomeRot{};
	float skydomeRotSp = 1.f;

	///�n��
	unique_ptr<Model> groundModel;
	unique_ptr<Object3D> groundObject;
	unique_ptr<Ground> ground;

	//�S�[��
	unique_ptr<Model> goalModel;
	unique_ptr<Object3D> goalObject;
	unique_ptr<Goal> goal;

	//�Փ˔���
	unique_ptr<CollisionManager> collisionManager;

	//���U���g
	unique_ptr<Result> result;
	std::unique_ptr<Sprite> backGround;
	std::unique_ptr<Sprite> RESULTtext;
	std::unique_ptr<Sprite> CLEARtext;
	std::unique_ptr<Sprite> RESTARTtext;
	std::unique_ptr<Sprite> coinSprite[3];

//protected
private:
#pragma region �p���ϐ�(�ėp�@�\)
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

