#pragma once
#include "Chunk.h"
#include "World.h"
#include "Input/InputManager.h"
#include "Utilities/Octree/Octree.h"

class ChunkRenderer {
public:
	ChunkRenderer();
	~ChunkRenderer();

	bool initialize(ID3D11Device* device, ID3D11DeviceContext * device_context);
	void draw(const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix, BoundingFrustum & frustum);
	void setWorld(World* world);
	void setEnableCulling(bool state);
private:

	DirectX::XMMATRIX makeModelMatrix(ChunkCoord  coord);
	void cullChunks(const DirectX::XMMATRIX & view_matrix, BoundingFrustum & frustum);
	void unloadChunks();
	void loadChunks();
	void rebuildChunks();

	World* world_;

	ConstantBuffer<CB_VS_object_buffer> CBVSObject_;

	std::unordered_map<ChunkCoord, Chunk*> activeChunks_;
	std::vector<ChunkCoord> loadList_;
	std::unordered_map<ChunkCoord, Chunk*> unloadList_;
	std::unordered_map<ChunkCoord, Chunk*> renderList_;
	std::vector<Chunk*> rebuildList_;

	ID3D11Device* device_;
	ID3D11DeviceContext * deviceContext_;

	bool enableCull_;
	InputContext chunkContext_;
	Texture * texture_;
	Octree<Chunk> octree_;
};

