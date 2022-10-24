#include "DebugArea.h"
#include <fstream>
#include <cassert>

using namespace std;

void DebugArea::AppInitialize(WinApp *winapp, DirectXCommon *dxCommon, Input *input, TextureManager *textureManager, GeometryManager *geometryModel, Camera *camera, DebugText *debugText, SoundManager *soundManager)
{
	this->winApp = winapp;
	this->dxCommon = dxCommon;
	this->input = input;
	this->textureManager = textureManager;
	this->geometryModel = geometryModel;
	this->camera = camera;
	this->debugText = debugText;
	this->soundManager = soundManager;
}

void DebugArea::Initialize()
{
#pragma region �I�u�W�F�N�g������

	/// <summary>
	/// �J����
	/// </summary>
	this->camera->CameraPosInitialize();

	/// <summary>
	/// �v���C���[
	/// </summary>
	playerModel = make_unique<Model>();
	playerModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("Player"));
	playerObject = make_unique<Object3D>();
	player = make_unique<Player>();
	player->Initialize(this->input, playerModel.get(), playerObject.get(), {0,10,0});

	/// <summary>
	/// �R�C��
	/// </summary>
	LoadCoinPopData();
	UpdateCoinPopCommands();

	/// <summary>
	/// ��
	/// </summary>
	LoadWallPopData();
	UpdateWallPopCommands();

	/// <summary>
	/// ���Ⴊ�ݕ�
	/// </summary>
	LoadSlideWallPopData();
	UpdateSlideWallPopCommands();

	/// <summary>
	/// �V��
	/// </summary>
	skydomeModel = make_unique<Model>();
	skydomeModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("skydome"));
	skydomeObject = make_unique<Object3D>();
	skydome = make_unique<Skydome>();
	skydome->Initialize(skydomeModel.get(), skydomeObject.get());
	skydomeRot = {0,0,0};

	/// <summary>
	/// �n��
	/// </summary>
	groundModel = make_unique<Model>();
	groundModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("Area02"));
	groundObject = make_unique<Object3D>();
	ground = make_unique<Ground>();
	ground->Initialize(groundModel.get(), groundObject.get(), ObjectsVelocity, {0,-10,0});

	/// <summary>
	/// �S�[��
	/// </summary>
	goalModel = make_unique<Model>();
	goalModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("goal"));
	goalObject = make_unique<Object3D>();
	goal = make_unique<Goal>();
	goal->Initialize(goalModel.get(), goalObject.get(), ObjectsVelocity, {0,-10,4100});
#pragma endregion 

#pragma region �X�v���C�g������
	result = make_unique<Result>();

	this->textureManager->LoadTexture(1, L"Resources/BackGround/TransparentBackGround.png");
	this->textureManager->LoadTexture(2, L"Resources/Text/CLEAR_TextFont.png");
	this->textureManager->LoadTexture(3, L"Resources/coinSprite/CoinBack.png");
	this->textureManager->LoadTexture(4, L"Resources/coinSprite/Coin.png");
	this->textureManager->LoadTexture(5, L"Resources/Text/RESULT_TextFont.png");
	this->textureManager->LoadTexture(6, L"Resources/HP/Hart.png");
	this->textureManager->LoadTexture(7, L"Resources/HP/HartBack.png");
	this->textureManager->LoadTexture(8, L"Resources/Button/buttonW.png");
	this->textureManager->LoadTexture(9, L"Resources/Button/buttonS.png");
	this->textureManager->LoadTexture(10, L"Resources/Button/buttonA.png");
	this->textureManager->LoadTexture(11, L"Resources/Button/buttonD.png");
	this->textureManager->LoadTexture(12, L"Resources/Button/buttonUp.png");
	this->textureManager->LoadTexture(13, L"Resources/Button/buttonDown.png");
	this->textureManager->LoadTexture(14, L"Resources/ScoreIncrement/CoinScoreIncrement.png");
	this->textureManager->LoadTexture(15, L"Resources/ShockEffect/ShockEffect.png");

	//HP
	for(int i = 0; i < player->GetMaxHp(); i++)
	{
		HpSprite[i] = make_unique<Sprite>();
		player->HpSpriteInitialize(HpSprite[i].get(), i);
	}
	//Button
	ButtonUp = make_unique<Sprite>();
	ButtonDown = make_unique<Sprite>();
	ButtonLeft = make_unique<Sprite>();
	ButtonRight = make_unique<Sprite>();
	ButtonW = make_unique<Sprite>();
	ButtonS = make_unique<Sprite>();
	player->BottomSpriteInitialize(ButtonUp.get(), ButtonDown.get(), ButtonLeft.get(), ButtonRight.get(), ButtonW.get(), ButtonS.get());

	//�w�i
	backGround = make_unique<Sprite>();
	backGround->Initialize(1);
	result->BackGroundSpriteInitialize(backGround.get());

	//���U���g
	RESULTtext = make_unique<Sprite>();
	RESULTtext->Initialize(5);
	result->ResultTextInitialize(RESULTtext.get());

	//�N���A
	CLEARtext = make_unique<Sprite>();
	CLEARtext->Initialize(2);
	result->ClearTextInitialie(CLEARtext.get());

	//���X�^�[�g
	RESTARTtext = make_unique<Sprite>();
	RESTARTtext->Initialize(5);
	result->RestartTextInitialize(RESTARTtext.get());

	//�R�C��
	for(int i = 0; i < 3; i++)
	{
		coinSprite[i] = make_unique<Sprite>();
		coinSprite[i]->Initialize(3);
		result->CoinSpriteInitialize(coinSprite[i].get(), i);
	}

	//�R�C�����ZUI
	coinIncrement = make_unique<Sprite>();
	player->CoinIncrementSpriteInitialize(coinIncrement.get());
	
	//�_���[�W�G�t�F�N�g
	damageEffect = make_unique<Sprite>();
	player->DamageEffectSpriteInitialize(damageEffect.get());

#pragma endregion

	//�����蔻��
	collisionManager = make_unique<CollisionManager>();
}

void DebugArea::Update()
{
#pragma region �I�u�W�F�N�g �X�V
	//�v���C���[ �X�V
	player->Update();
	result->SetCoinNum(player->GetCoins());
	result->SetIsGoal(player->GetIsGoal());
	if(player->GetHp() <= 0)
	{
		result->SetIsDead(true);
	}

	//�R�C��
	//�폜
	coin.remove_if([](unique_ptr<Coin>& obj){
		return obj->GetIsDead();
	});
	for(unique_ptr<Coin>& obj : coin){
		obj->Update();
	}

	//��
	//�폜
	wall.remove_if([](unique_ptr<Obstacle>& obj){
		return obj->GetIsDead();
	});
	for(unique_ptr<Obstacle>& obj : wall){
		obj->Update();
	}

	//���Ⴊ�ݕ�
	//�폜
	lslideWall.remove_if([](unique_ptr<slideWall>& obj){
		return obj->GetIsDead();
	});
	for(unique_ptr<slideWall>& obj : lslideWall){
		obj->Update();
	}

	//�V�� �X�V
	skydomeRot += {0,XMConvertToRadians(skydomeRotSp),0};
	skydomeObject->SetRotation(skydomeRot);
	skydomeObject->Update();
	skydome->Update();
	//�n�� �X�V
	ground->Update();
	//�S�[�� �X�V
	goal->Update();
#pragma endregion

#pragma region �X�v���C�g
	//�v���C���[
	player->SpriteUpdate(camera->GetMatView(), camera->GetMatProjection());
#pragma endregion

#pragma region �ėp�@�\ �X�V
	this->camera->Update();
#pragma endregion

#pragma region �V�[�� �X�V

	//���U���g
	GoalUpdate();
	DeadUpdate();

	//�V�[���J��
	if(result->GetIsSceneChange())
	{
		if(input->Trigger(DIK_SPACE) || input->Trigger(DIK_RETURN))
		{
			sceneChange = true;
			sceneChangeNumber = SCENE::SELECT;
		}
	}

	//�Փ˔���X�V
	CheckAllCollision();
#pragma endregion

#ifdef _DEBUG

	if(input->Trigger(DIK_SPACE))
	{
		sceneChange = true;
		sceneChangeNumber = SCENE::SELECT;
	}

#endif // _DEBUG
}

void DebugArea::Draw()
{
	this->dxCommon->BeginDraw();

	///3D�`��
#pragma region �I�u�W�F�N�g_3D�`��
	//�v���C���[ �`��
	player->Draw(dxCommon->GetCommandList());

	//�R�C�� �`��
	for(unique_ptr<Coin>& obj : coin){
		obj->Draw(this->dxCommon->GetCommandList());
	}

	//�� �`��
	for(unique_ptr<Obstacle>& obj : wall){
		obj->Draw(this->dxCommon->GetCommandList());
	}

	//���Ⴊ�ݕ� �`��
	for(unique_ptr<slideWall>& obj : lslideWall){
		obj->Draw(this->dxCommon->GetCommandList());
	}

	//�V�� �`��
	skydome->Draw(dxCommon->GetCommandList());
	//�n�� �`��
	ground->Draw(dxCommon->GetCommandList());
	//�S�[�� �`��
	goal->Draw(dxCommon->GetCommandList());
#pragma endregion

	///2D�`��
	Sprite::SetPipelineState(this->dxCommon->GetCommandList());
#pragma region �����o_2D�`��
	//Hp
	player->SpriteDraw(this->dxCommon->GetCommandList());

	//���U���g
	result->GoalDraw(this->dxCommon->GetCommandList());
#pragma endregion

#ifdef _DEBUG

#endif // _DEBUG

	this->dxCommon->EndDraw();
}

void DebugArea::Finalize()
{
}

#pragma region �R�C������
void DebugArea::LoadCoinPopData()
{
	//file�J��
	ifstream file;
	file.open("Resources/csv/Area01/CoinPopData.csv");
	assert(file.is_open());

	//file�|�{�𕶎���X�g���[���ɃR�s�[
	coinPopCommands << file.rdbuf();
	file.close();
}
void DebugArea::UpdateCoinPopCommands()
{
	//��s���̕����������ϐ�
	string line;

	//�R�}���h���[�v
	while(getline(coinPopCommands, line)){
		//��s���̂���������X�g���[���ɕϊ����ĉ�͂��₷��
		istringstream line_stream(line);

		string word;
		//��؂�ōs�̑K��������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�̂�comment
		if(word.find("//") == 0){
			//��΂�
			continue;
		}

		//POP�R�}���h
		if(word.find("POP") == 0){
			//X���W
			getline(line_stream, word, ',');
			float x = (float)atof(word.c_str());

			//Y���W
			getline(line_stream, word, ',');
			float y = (float)atof(word.c_str());

			//Z���W
			getline(line_stream, word, ',');
			float z = (float)atof(word.c_str());

			//POP
			CoinPop(Vector3(x,y,z));
		}
	}
}
void DebugArea::CoinPop(Vector3 pos)
{
	unique_ptr<Model> newcoinModel = make_unique<Model>();
	newcoinModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("coin"));
	unique_ptr<Object3D> newcoinObject = make_unique<Object3D>();
	unique_ptr<Coin> newcoin = make_unique<Coin>();
	newcoin->Initialize(newcoinModel.get(), newcoinObject.get(), ObjectsVelocity, pos);

	coin.push_back(move(newcoin));
	coinModel.push_back(move(newcoinModel));
	coinObject.push_back(move(newcoinObject));
}
#pragma endregion

#pragma region �Ǐ���
void DebugArea::LoadWallPopData()
{
	//file�J��
	ifstream file;
	file.open("Resources/csv/Area01/WallPopData.csv");
	assert(file.is_open());

	//file�|�{�𕶎���X�g���[���ɃR�s�[
	wallPopCommands << file.rdbuf();
	file.close();
}
void DebugArea::UpdateWallPopCommands()
{
	//��s���̕����������ϐ�
	string line;

	//�R�}���h���[�v
	while(getline(wallPopCommands, line)){
		//��s���̂���������X�g���[���ɕϊ����ĉ�͂��₷��
		istringstream line_stream(line);

		string word;
		//��؂�ōs�̑K��������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�̂�comment
		if(word.find("//") == 0){
			//��΂�
			continue;
		}

		//POP�R�}���h
		if(word.find("POP") == 0){
			//Z���W
			getline(line_stream, word, ',');
			float z = (float)atof(word.c_str());

			//POP
			//01�Ԓn
			getline(line_stream, word, ',');
			bool Is01 = (float)atof(word.c_str());
			if(Is01){WallPop(Vector3(-70,10,z));}

			//02�Ԓn
			getline(line_stream, word, ',');
			bool Is02 = (float)atof(word.c_str());
			if(Is02){WallPop(Vector3(0,10,z));}

			//02�Ԓn
			getline(line_stream, word, ',');
			bool Is03 = (float)atof(word.c_str());
			if(Is03){WallPop(Vector3(70,10,z));}
		}
	}
}
void DebugArea::WallPop(Vector3 pos)
{
	unique_ptr<Model> newModel = make_unique<Model>();
	newModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("obstacle"));
	unique_ptr<Object3D> newObject = make_unique<Object3D>();
	unique_ptr<Obstacle> newwall = make_unique<Obstacle>();
	newwall->Initialize(newModel.get(), newObject.get(), ObjectsVelocity, pos);

	wall.push_back(move(newwall));
	wallModel.push_back(move(newModel));
	wallObject.push_back(move(newObject));
}
#pragma endregion

#pragma region �X���C�h��
void DebugArea::LoadSlideWallPopData()
{
	//file�J��
	ifstream file;
	file.open("Resources/csv/Area01/SlideWallPopData.csv");
	assert(file.is_open());

	//file�|�{�𕶎���X�g���[���ɃR�s�[
	slideWallPopCommands << file.rdbuf();
	file.close();
}
void DebugArea::UpdateSlideWallPopCommands()
{
	//��s���̕����������ϐ�
	string line;

	//�R�}���h���[�v
	while(getline(slideWallPopCommands, line)){
		//��s���̂���������X�g���[���ɕϊ����ĉ�͂��₷��
		istringstream line_stream(line);

		string word;
		//��؂�ōs�̑K��������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�̂�comment
		if(word.find("//") == 0){
			//��΂�
			continue;
		}

		//POP�R�}���h
		if(word.find("POP") == 0){
			//Z���W
			getline(line_stream, word, ',');
			float z = (float)atof(word.c_str());

			//POP
			//01�Ԓn
			getline(line_stream, word, ',');
			bool Is01 = (float)atof(word.c_str());
			if(Is01){SlideWallPop(Vector3(-70,16.f,z));}

			//02�Ԓn
			getline(line_stream, word, ',');
			bool Is02 = (float)atof(word.c_str());
			if(Is02){SlideWallPop(Vector3(0,16.f,z));}

			//02�Ԓn
			getline(line_stream, word, ',');
			bool Is03 = (float)atof(word.c_str());
			if(Is03){SlideWallPop(Vector3(70,16.f,z));}
		}
	}
}
void DebugArea::SlideWallPop(Vector3 pos)
{
	unique_ptr<Model> newModel = make_unique<Model>();
	newModel = unique_ptr<Model>(FbxLoader::GetInstance()->LoadModeFromFile("slideWall"));
	unique_ptr<Object3D> newObject = make_unique<Object3D>();
	unique_ptr<slideWall> lnew = make_unique<slideWall>();
	lnew->Initialize(newModel.get(), newObject.get(), ObjectsVelocity, pos);

	lslideWall.push_back(move(lnew));
	slideWallModel.push_back(move(newModel));
	slideWallObject.push_back(move(newObject));
}
#pragma endregion

#pragma region �Փ˔���
void DebugArea::CheckAllCollision()
{
	//���X�g�N���A
	collisionManager->CollisionClear();
	//���X�g�ǉ�
	collisionManager->SetCollision(player.get());
	collisionManager->SetCollision(goal.get());
	for(const std::unique_ptr<Coin>& obj : coin)
	{
		collisionManager->SetCollision(obj.get());
	}
	for(const std::unique_ptr<Obstacle>& obj : wall)
	{
		collisionManager->SetCollision(obj.get());
	}
	for(const std::unique_ptr<slideWall>& obj : lslideWall)
	{
		collisionManager->SetCollision(obj.get());
	}

	//�������蔻��
	//���X�g���̃y�A�𑍓�����
	std::list<Collider*>::iterator itrA = collisionManager->colliders.begin();
	for(; itrA != collisionManager->colliders.end(); ++itrA){
		//�C�e���[�^A����R���C�_�[A���擾
		Collider* colliderA = *itrA;

		//�C�e���[�^B�̓C�e���[�^A�̎��̗v�f�����(�d��������)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for(; itrB != collisionManager->colliders.end(); ++itrB){
			//�C�e���[�^B����R���C�_�[B���擾
			Collider* colliderB = *itrB;

			//�y�A�̓����蔻��
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}
void DebugArea::CheckCollisionPair(Collider *colliderA, Collider *colliderB)
{
	//�Փ˃t�B���^�����O
	if(colliderA->GetCollisionAttribute() != colliderB->GetCollisionMask() || colliderB->GetCollisionAttribute() != colliderA->GetCollisionMask()){
		return;
	}

	if(CheckCollisionDetail(colliderA, colliderB)){
		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);
	}
}
bool DebugArea::CheckCollisionDetail(Collider *colliderA, Collider *colliderB)
{
	//�{�b�N�X���m�̓����蔻��
	if(colliderA->GetPosition().x-colliderA->GetWidth() > colliderB->GetPosition().x+colliderB->GetWidth())		return false;
	if(colliderA->GetPosition().x+colliderA->GetWidth() < colliderB->GetPosition().x-colliderB->GetWidth())		return false;
	if(colliderA->GetPosition().y-colliderA->GetHeight() > colliderB->GetPosition().y+colliderB->GetHeight())	return false;
	if(colliderA->GetPosition().y+colliderA->GetHeight() < colliderB->GetPosition().y-colliderB->GetHeight())	return false;
	if(colliderA->GetPosition().z-colliderA->GetWidth() > colliderB->GetPosition().z+colliderB->GetWidth())		return false;
	if(colliderA->GetPosition().z+colliderA->GetWidth() < colliderB->GetPosition().z-colliderB->GetWidth())		return false;

	return true;
}
#pragma endregion

#pragma region �S�[������
void DebugArea::GoalUpdate()
{
	//���U���g����
	result->GoalUpdate();

	if(!result->GetIsGoal()) return ;
	ObjectVelocityStop();
}
void DebugArea::DeadUpdate()
{
	result->GoalUpdate();

	if(!result->GetIsDead()) return ;
	ObjectVelocityStop();
}
void DebugArea::ObjectVelocityStop()
{
	//���x�ݒ�
	ground->SetVelocity(0.f);
	goal->SetVelocity(0.f);
	for(const unique_ptr<Coin>& obj : coin){
		obj->SetVelocity(0.f);
	}
	for(const unique_ptr<Obstacle>& obj : wall){
		obj->SetVelocity(0.f);
	}
	for(const unique_ptr<slideWall>& obj : lslideWall){
		obj->SetVelocity(0.f);
	}
}
#pragma endregion
