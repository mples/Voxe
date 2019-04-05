#pragma once
#include "BlockType.h"
#include "Graphics/WorldChunkModel.h"

using namespace DirectX;

class Chunk {
public:
//	using byte4 = glm::tvec4<GLbyte>;
	static const unsigned int DIM = 16;

	Chunk();
	~Chunk();

	bool initialize(ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader, Texture * tex);
	void update();

	void draw(XMMATRIX model_matrix, XMMATRIX view_proj_matrix);

	void setBlock(int x, int y, int z, BlockType type);
	bool chagned();
	bool isEmpty();
	BlockType getBlock(int x, int y, int z);
	Chunk* left_, *right_, *up_, *down_, *front_, *back_;
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

	void calculateVertices(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);

	const XMFLOAT3 POS_X_NORMAL = XMFLOAT3(1.0f, 0.0f, 0.0f);
	const XMFLOAT3 NEG_X_NORMAL = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	const XMFLOAT3 POS_Y_NORMAL = XMFLOAT3(0.0f, 1.0f, 0.0f);
	const XMFLOAT3 NEG_Y_NORMAL = XMFLOAT3(0.0f, -1.0f, 0.0f);
	const XMFLOAT3 POS_Z_NORMAL = XMFLOAT3(0.0f, 0.0f, 1.0f);
	const XMFLOAT3 NEG_Z_NORMAL = XMFLOAT3(-1.0f, 0.0f, -1.0f);

	BlockType blocks_[DIM][DIM][DIM];
	bool changed_;
	
	int elements_;

	WorldChunkModel model_;
	bool isCovered(int x, int y, int z, int cov_x, int cov_y, int cov_z);
};

