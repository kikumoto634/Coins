#include "SceneManager.h"

#include "Title.h"
#include "DirectXGame.h"

GameBase *SceneManager::m_scene = NULL;
bool SceneManager::endFlag = false;

void SceneManager::Change(Scene scene)
{
}

void SceneManager::Initialize()
{
	m_scene = new Title();
}

void SceneManager::Update()
{
	endFlag = m_scene->GetEndFlag();
	m_scene->Update();
}

void SceneManager::Draw()
{
	m_scene->Draw();
}

void SceneManager::Finalize()
{
	m_scene->Finalize();
}
