#include "Engine.h"
#include "GameStage.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstace,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow) {


	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Error: Failed to coinitialize.");
		return false;
	}
	Engine engine;
	if (engine.init(hInstace, L"Title", L"MyWindowClass", 800, 800) ) {
		STAGE_MANAGER.addStage(StageType::GAME, new TStageBuilder<GameStage>());
		STAGE_MANAGER.setStartingStage(StageType::GAME);
		while (engine.processMessages() == true) {
			engine.update();
		}
	}

	return 0;
}