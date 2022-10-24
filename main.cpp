#include "DirectXGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE,LPSTR,int)
{
	auto game = new DirectXGame();

	//ゲーム実行
	game->Run();
	//解放
	delete game;

	return 0;
}
