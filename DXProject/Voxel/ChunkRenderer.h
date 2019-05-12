#pragma once
#include "Chunk.h"
#include "World.h"
#include "../Input/InputManager.h"
#include "../Utilities/Structures/Octree/Octree.h"
#include "../Utilities/ObjectPool.h"
#include "../Utilities/JobSystem.h"

#include "../BoundingVolume.h"
#include "../Graphics/VertexShader.h"
#include "../Graphics/PixelShader.h"

#include "../OcclusionCulling.h"
//class OcclusionCulling;

class ChunkRenderer {
public:
	ChunkRenderer();
	~ChunkRenderer();

	bool initialize(ID3D11Device* device, ID3D11DeviceContext * device_context);
	void initializeRenderState();
	bool initializeShaders();

	void applyRendererState();
	void draw(const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix, BoundingFrustum & frustum);
	void setWorld(World* world);
	void setEnableCulling(bool state);
	bool getEnableCull();
	std::unordered_map<ChunkCoord, Chunk*> activeChunks_;
private:

	DirectX::XMMATRIX makeModelMatrix(ChunkCoord  coord);
	void cullChunks(const DirectX::XMMATRIX & view_matrix, BoundingFrustum & frustum);
	void unloadChunks();
	void loadChunks();
	void rebuildChunks();
	void initializeChunks();

	void setAdjacentChunks(Chunk* chunk);

	// extending world
	void extendPosX();
	void extendNegX();
	void extendPosY();
	void extendNegY();
	void extendPosZ();
	void extendNegZ();

	World* world_;
	bool firstDraw_ = true;
	Octree<Chunk> octree_; 

	std::vector<Chunk*> initList_;
	std::queue<ChunkCoord> loadList_;
	std::queue<ChunkCoord> unloadList_;
	std::vector<Chunk*> renderList_;
	std::vector<Chunk*> rebuildList_;

	ID3D11Device* device_;
	ID3D11DeviceContext * deviceContext_;
	ConstantBuffer<CB_VS_object_buffer> CBVSObject_;
	Texture * texture_;

	XMFLOAT3 previousCenter_;
	XMINT3 minBounds_;
	XMINT3 maxBounds_;

	bool enableCull_;
	InputContext chunkContext_;

	ObjectPool<Chunk, 100000> chunkPool_;

	BoundingVolume boundingVolume_;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState_;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState_;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> * renderTargetView_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> * depthStencilView_;

	VertexShader vertexShader_;
	PixelShader pixelShader_;

	OcclusionCulling occlusionCulling_;
};

