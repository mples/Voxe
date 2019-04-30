#pragma once
#include "Chunk.h"
#include "World.h"
#include "Input/InputManager.h"
#include "Utilities/Octree/Octree.h"
#include "ObjectPool.h"

class ChunkRenderer {
public:
	ChunkRenderer();
	~ChunkRenderer();

	bool initialize(ID3D11Device* device, ID3D11DeviceContext * device_context);
	void draw(const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix, BoundingFrustum & frustum);
	void setWorld(World* world);
	void setEnableCulling(bool state);
	Octree<Chunk> octree_; // TODO move back to private
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
};

