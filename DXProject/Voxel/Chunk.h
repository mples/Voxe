#pragma once
#include <DirectXCollision.h>

#include "BlockType.h"
#include "../Graphics/WorldChunkModel.h"
#include "ChunkCoord.h"
#include "../BoundingVolume.h"

using namespace DirectX;

class Chunk;

struct ChunkNeighbours {
	Chunk* left_, *right_, *up_, *down_, *front_, *back_;
};

class Chunk {
public:
	static const unsigned int DIM = 16;

	Chunk();
	Chunk(int x, int y, int z);
	Chunk(int x, int y, int z, BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM]);
	Chunk(ChunkCoord coord, BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM]);
	~Chunk();

	void initialize();
	void initialize(int x, int y, int z, BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM]);
	void initialize(ChunkCoord coord, BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM]);

	bool initializeMesh(ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& const_buffer, Texture * texture);
	void update(ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& const_buffer, Texture * texture);

	void draw(XMMATRIX view_proj_matrix);

	void setBlock(int x, int y, int z, BlockType type);
	BlockType getBlock(UINT x, UINT y, UINT z);
	BoundingBox & getBoundingVolume();
	XMMATRIX & getWorldMatrix();
	ChunkCoord & getCoord();
	XMINT3 getPos();
	BoundingVolume & getBV();

	ChunkNeighbours neighbours_;
	bool initialized_;
	bool isEmpty_;
	bool changed_;
	bool visible_ = true;
private:
	void insertNegativeX(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);
	void insertPositiveX(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);

	void insertNegativeY(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);
	void insertPositiveY(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);

	void insertNegativeZ(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);
	void insertPositiveZ(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);

	bool isObscuredNegativeX(UINT x, UINT y, UINT z);
	bool isObscuredPositiveX(UINT x, UINT y, UINT z);
	bool isObscuredNegativeY(UINT x, UINT y, UINT z);
	bool isObscuredPositiveY(UINT x, UINT y, UINT z);
	bool isObscuredNegativeZ(UINT x, UINT y, UINT z);
	bool isObscuredPositiveZ(UINT x, UINT y, UINT z);

	void calculateVertices(UINT x, UINT y, UINT z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);

	void calculateMeshData(std::vector<Vertex> &vertices, std::vector<DWORD> &indices);

	const XMFLOAT3 POS_X_NORMAL = XMFLOAT3(1.0f, 0.0f, 0.0f);
	const XMFLOAT3 NEG_X_NORMAL = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	const XMFLOAT3 POS_Y_NORMAL = XMFLOAT3(0.0f, 1.0f, 0.0f);
	const XMFLOAT3 NEG_Y_NORMAL = XMFLOAT3(0.0f, -1.0f, 0.0f);
	const XMFLOAT3 POS_Z_NORMAL = XMFLOAT3(0.0f, 0.0f, 1.0f);
	const XMFLOAT3 NEG_Z_NORMAL = XMFLOAT3(-1.0f, 0.0f, -1.0f);

	BlockType blocks_[DIM][DIM][DIM];
	
	WorldChunkModel model_;

	BoundingBox worldBoundingBox_;
	ChunkCoord coord_;
	XMMATRIX worldMatrix_;
	BoundingVolume boundingVolume_;

};

