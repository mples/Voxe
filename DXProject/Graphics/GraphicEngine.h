#pragma once
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include "AdapterReader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"
#include "../Timer.h"
#include "RenderableObject.h"
#include "Light.h"

#include "../World.h"
#include "../ChunkRenderer.h"
#include "../Generation/PerlinNoise.h"
#include "../Generation/SimpleGenerator.h"

class GraphicEngine {
public:
	GraphicEngine();
	~GraphicEngine();
	bool initialize(HWND hwnd, int width, int height);
	void draw();

	Camera camera_;
	Light light_;
private:
	bool initializeDirectX(HWND hwnd);
	bool initializeShaders();
	bool initializeScene();

	bool enableMsaa_;
	Microsoft::WRL::ComPtr<ID3D11Device> device_;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext_;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView_;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState_;

	VertexShader vertexShader_;	
	PixelShader pixelShader_;
	ConstantBuffer<CB_VS_object_buffer> VSconstantBuffer_;
	ConstantBuffer<CB_PS_point_light> PSpointLightBuffer_;
	ConstantBuffer<CB_PS_frame_buffer> PSframeBuffer_;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;
	std::unique_ptr<DirectX::SpriteFont> spriteFont_;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState_;

	int windowWidth_;
	int windowHeight_;
	Timer fpsTimer_;

	RenderableObject gameObject_;
	Chunk chunk_;
	ChunkRenderer chunkRenderer_;
	World * world_;
	SimpleGenerator * simpleGenerator_;
	PerlinNoise * perlinNoise_;
};

