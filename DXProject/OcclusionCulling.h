#pragma once
#include<d3d11.h>
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
//#include "Graphics/GraphicEngine.h"
#include "BoundingVolume.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/RenderableObject.h"

#include "SpatialHash.h"
#include "Voxel/Chunk.h"

class OcclusionCulling {
public:
	OcclusionCulling();
	~OcclusionCulling();

	bool initialize(ID3D11Device * device, ID3D11DeviceContext * device_context);
	bool initializeShaders(ID3D11Device * device);

	void cull(const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix, std::vector<Chunk*>& chunks);

private:
	XMFLOAT3 getCameraFrontDir(const DirectX::XMMATRIX & view_matrix);

	ID3D11Device * device_;
	ID3D11DeviceContext * deviceContext_;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState_;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState_;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_;

	UINT depthBufferHeight_ = 512;
	UINT depthBufferWidth_ = 512;
	VertexShader vertexShader_;
	BoundingVolume boundingVolume_;
	ConstantBuffer<CB_VS_object_buffer> objectBuffer_;
	RenderableObject object_;
	SpatialHash<BoundingVolume> spatialMap_;
};

