#pragma once
#include "GameBase.h"
class SceneManager
{
public:
	enum class Scene
	{
		Title, 
		INGame
	};

	static bool endFlag;

public:
	static void Change(Scene scene);
	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

private:
	static GameBase* m_scene;
};

