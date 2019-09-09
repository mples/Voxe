#include "Engine.h"
#include "Game/GameStage.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstace,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow) {


	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Error: Failed to coinitialize.");
		return false;
	}
	Engine::createInstance();

	if (ENGINE.init(hInstace, L"Title", L"MyWindowClass", 800, 800) ) {
		STAGE_MANAGER.addStage(StageType::GAME, new TStageBuilder<GameStage>());
		STAGE_MANAGER.setStartingStage(StageType::GAME);
		while (ENGINE.processMessages() == true) {
			ENGINE.update();
		}
	}

	Engine::removeInstance();
	return 0;
}