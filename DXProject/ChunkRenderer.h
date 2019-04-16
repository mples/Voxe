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
	Octree<Chunk> octree_; // TODO move back to private
	std::vector<Chunk*> activeChunks_;
private:

	DirectX::XMMATRIX makeModelMatrix(ChunkCoord  coord);
	void cullChunks(const DirectX::XMMATRIX & view_matrix, BoundingFrustum & frustum);
	void unloadChunks();
	void loadChunks();
	void rebuildChunks();
	void initializeChunks();

	World* world_;
	bool firstDraw_ = true;

	std::vector<Chunk*> initList_;
	std::vector<ChunkCoord> loadList_;
	std::vector<ChunkCoord> unloadList_;
	std::vector<Chunk*> renderList_;
	std::vector<Chunk*> rebuildList_;

	ID3D11Device* device_;
	ID3D11DeviceContext * deviceContext_;
	ConstantBuffer<CB_VS_object_buffer> CBVSObject_;
	Texture * texture_;

	XMFLOAT3 previousExtend_;
	XMFLOAT3 previousCenter_;
	bool enableCull_;
	InputContext chunkContext_;
	
};

