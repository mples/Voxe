#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../Graphics/AdapterReader.h"
#include "../ECS/System.h"
#include "../Graphics/Texture.h"
#include "../Graphics/BlocksTextureAtlas.h"
#include "../Graphics/VertexShader.h"
#include "../Graphics/PixelShader.h"
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/ConstantBufferTypes.h"

#include "../Components/MeshComponent.h"
#include "../Components/WorldCoordinateComponent.h"

#include "../ECS/Event/IEventListener.h"

#include "../Events/CameraCreated.h"
#include "../Events/CameraDestroyed.h"
#include "../Events/FogChangeRequest.h"
#include "../Events/SetPointLightRequest.h"

#include "../Entities/GameCamera.h"

class RenderSystem : public System<RenderSystem>, public IEventListener {
public:
	RenderSystem(HWND hwd, int width, int height);
	~RenderSystem();

	virtual void preUpdate(float dt) override;

	virtual void update(float dt) override;

	virtual void postUpdate(float dt) override;

	void createTextureAtlas(std::wstring texture_atlas_path, const std::vector<BlockType> block_types);
private:
	void initializeDirectX(HWND hwnd);
	void initializeRenderState();
	bool initializeShaders();

	void drawObject(MeshComponent * mesh, WorldCoordinateComponent* coord);

	void onSetPointLightRequest(const SetPointLightRequest * e);
	void onFogChangeRequest(const FogChangeRequest * e);
	void onCameraCreatedEvent(const CameraCreated * e);
	void onCameraDestroyedEvent(const CameraDestroyed * e);

	int windowWidth_;
	int windowHeight_;

	bool enableMsaa_;
	Microsoft::WRL::ComPtr<ID3D11Device> device_;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext_;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer_;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;
	std::unique_ptr<DirectX::SpriteFont> spriteFont_;

	D3D11_VIEWPORT viewport_;

	//Render state
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState_;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStateOff_;


	VertexShader vertexShader_;
	PixelShader pixelShader_;

	ConstantBuffer<CB_VS_object_buffer> objectBufferVS_;

	ConstantBuffer<CB_PS_fog_buffer> fogBuffer_;

	ConstantBuffer<CB_PS_frame_buffer> frameBufferPS_;

	ConstantBuffer<CB_PS_point_light> pointLightBuffer_;

	Texture * invalidTexture_;

	BlocksTextureAtlas * blocksTextureAtlas_;

	IGameCamera * activeCamera_;
};