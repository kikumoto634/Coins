#include "DirectXGame.h"
#include <fstream>
#include <cassert>

#include "Title.h"
#include "Select.h"
#include "DebugArea.h"
#include "DebugArea02.h"
#include "Tutorial01.h"
#include "Tutorial02.h"
#include "Tutorial03.h"
#include "Tutorial04.h"
#include "Tutorial05.h"
#include "Area01.h"
#include "Area02.h"
#include "Area03.h"
#include "Area04.h"

using namespace std;

void DirectXGame::AppInitialize(WinApp* winapp, DirectXCommon* dxCommon, Input* input, TextureManager* textureManager, 
		GeometryManager* geometryModel, Camera* camera, DebugText* debugText, SoundManager* soundManager)
{
	this->winApp = winapp;
	this->dxCommon = dxCommon;
	this->input = input;
	this->textureManager = textureManager;
	this->geometryModel = geometryModel;
	this->camera = camera;
	this->debugText = debugText;
	this->soundManager = soundManager;

	scene = make_unique<Title>();
	sceneNumber = SCENE::TITLE;
	scene->AppInitialize(this->winApp, this->dxCommon, this->input, this->textureManager, this->geometryModel, this->camera, this->debugText, this->soundManager);
}

void DirectXGame::Initialize()
{
	//基底クラスの初期化
	scene->Initialize();
}

void DirectXGame::Update()
{
	//基底クラスの更新
	GameBase::Update();

	SceneChange();
	scene->Update();
}

void DirectXGame::Draw()
{
	scene->Draw();
}

void DirectXGame::Finalize()
{
	//基底クラスの解放
	GameBase::Finalize();

	scene->Finalize();
}

void DirectXGame::SceneChange()
{
	if(scene->GetSceneChange())
	{
		//TITLE -> SELECT
		if(sceneNumber == SCENE::TITLE)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		else if(sceneNumber == SCENE::SELECT)
		{
			switch (scene->GetSceneChangeNumber())
			{
			#ifdef _DEBUG
				//SELECT->DebugArea
			case SCENE::DEBUGAREA:
				scene = make_unique<DebugArea>();
				sceneNumber = SCENE::DEBUGAREA;
				break;
			case SCENE::DEBUGAREA02:
				scene = make_unique<DebugArea02>();
				sceneNumber = SCENE::DEBUGAREA02;
				break;
			#endif // DEBUG

				//SELECT->Tutorial01
			case SCENE::TUTORIAL01:
				scene = make_unique<Tutorial01>();
				sceneNumber = SCENE::TUTORIAL01;
				break;

				//SELECT->Tutorial02
			case SCENE::TUTORIAL02:
				scene = make_unique<Tutorial02>();
				sceneNumber = SCENE::TUTORIAL02;
				break;

				//SELECT->Tutorial03
			case SCENE::TUTORIAL03:
				scene = make_unique<Tutorial03>();
				sceneNumber = SCENE::TUTORIAL03;
				break;

				//SELECT->Tutorial04
			case SCENE::TUTORIAL04:
				scene = make_unique<Tutorial04>();
				sceneNumber = SCENE::TUTORIAL04;
				break;

				//SELECT->Tutorial05
			case SCENE::TUTORIAL05:
				scene = make_unique<Tutorial05>();
				sceneNumber = SCENE::TUTORIAL05;
				break;

			//SELECT->Area01
			case SCENE::AREA01:
				scene = make_unique<DebugArea>();
				sceneNumber = SCENE::AREA01;
				break;

			//SELECT->Area02
			case SCENE::AREA02:
				scene = make_unique<Area02>();
				sceneNumber = SCENE::AREA02;
				break;

			//SELECT->Area03
			case SCENE::AREA03:
				scene = make_unique<Area03>();
				sceneNumber = SCENE::AREA03;
				break;

			//SELECT->Area04
			case SCENE::AREA04:
				scene = make_unique<Area04>();
				sceneNumber = SCENE::AREA04;
				break;

			default:
				break;
			}

		}
#ifdef _DEBUG
		//DEBUGAREA->SELECT
		else if(sceneNumber == SCENE::DEBUGAREA)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		else if(sceneNumber == SCENE::DEBUGAREA02)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
#endif // DEBUG
		//Tutorial01->SELECT
		else if(sceneNumber == SCENE::TUTORIAL01)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		//Tutorial02->SELECT
		else if(sceneNumber == SCENE::TUTORIAL02)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		//Tutorial03->SELECT
		else if(sceneNumber == SCENE::TUTORIAL03)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		//Tutorial04->SELECT
		else if(sceneNumber == SCENE::TUTORIAL04)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		//Tutorial05->SELECT
		else if(sceneNumber == SCENE::TUTORIAL05)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		//Area01->SELECT
		else if(sceneNumber == SCENE::AREA01)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		//Area02->SELECT
		else if(sceneNumber == SCENE::AREA02)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		//Area03->SELECT
		else if(sceneNumber == SCENE::AREA03)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}
		//Area04->SELECT
		else if(sceneNumber == SCENE::AREA04)
		{
			scene = make_unique<Select>();
			sceneNumber = SCENE::SELECT;
		}

		sceneChange = false;
		sceneChangeNumber = SCENE::NONE;
		scene->AppInitialize(this->winApp, this->dxCommon, this->input, this->textureManager, this->geometryModel, this->camera, this->debugText, this->soundManager);
		scene->Initialize();
	}
}
