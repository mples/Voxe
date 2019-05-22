#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../Graphics/AdapterReader.h"
#include "../ECS/System.h"

class RenderSystem : public System<RenderSystem> {
public:
	RenderSystem(HWND hwd, int width, int height);
	~RenderSystem();

	virtual void preUpdate(float dt) override;

	virtual void update(float dt) override;

	virtual void postUpdate(float dt) override;
private:

	void initializeDirectX(HWND hwnd);

	int windowWidth_;
	int windowHeight_;

	bool enableMsaa_;
	float a;
	Microsoft::WRL::ComPtr<ID3D11Device> device_;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext_;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer_;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;
	std::unique_ptr<DirectX::SpriteFont> spriteFont_;
};