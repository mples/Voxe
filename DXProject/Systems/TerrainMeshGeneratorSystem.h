#pragma once

#include "../Engine.h"
#include "../ECS/System.h"
#include "../ECS/Event/IEventListener.h"
#include "../Events/VoxelDataGeneratedEvent.h"
#include "../Events/DirectXDeviceCreated.h"
#include "../Entities/TerrainChunk.h"
#include "../Components/BlocksDataComponent.h"
#include "../Components/TerrainNeighboursComponent.h"

#include "../Graphics/Vertex.h"

#include <d3d11.h>
#include <list>

class TerrainMeshGenerationSystem : public System<TerrainMeshGenerationSystem>, public IEventListener {
	struct NeightbourData {
		BlocksDataComponent * left_;
		BlocksDataComponent * right_;
		BlocksDataComponent * top_;
		BlocksDataComponent * bottom_;
		BlocksDataComponent * front_;
		BlocksDataComponent * back_;
	};
public:
	TerrainMeshGenerationSystem();
	~TerrainMeshGenerationSystem();

	virtual void update(float dt) override;
private:
	void onVoxelDataGeneratedEvent(const VoxelDataGeneratedEvent* e);

	void onDirectXDeviceCreated(const DirectXDeviceCreated * e);

	void calculateVertices(UINT x, UINT y, UINT z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices, BlocksDataComponent * blocks_com, NeightbourData neight_data);

	void calculateMeshData(std::vector<Vertex> &vertices, std::vector<DWORD> &indices, BlocksDataComponent * blocks_com, NeightbourData neight_data);

	void insertNegativeX(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);
	void insertPositiveX(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);

	void insertNegativeY(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);
	void insertPositiveY(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);

	void insertNegativeZ(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);
	void insertPositiveZ(float x, float y, float z, BlockType type, std::vector<Vertex> &vertices, std::vector<DWORD> &indices);

	bool isObscuredNegativeX(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data);
	bool isObscuredPositiveX(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data);
	bool isObscuredNegativeY(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data);
	bool isObscuredPositiveY(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data);
	bool isObscuredNegativeZ(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data);
	bool isObscuredPositiveZ(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data);


	const XMFLOAT3 POS_X_NORMAL = XMFLOAT3(1.0f, 0.0f, 0.0f);
	const XMFLOAT3 NEG_X_NORMAL = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	const XMFLOAT3 POS_Y_NORMAL = XMFLOAT3(0.0f, 1.0f, 0.0f);
	const XMFLOAT3 NEG_Y_NORMAL = XMFLOAT3(0.0f, -1.0f, 0.0f);
	const XMFLOAT3 POS_Z_NORMAL = XMFLOAT3(0.0f, 0.0f, 1.0f);
	const XMFLOAT3 NEG_Z_NORMAL = XMFLOAT3(-1.0f, 0.0f, -1.0f);

	ID3D11Device * device_;

	std::list<EntityId> entitiesToUpdate_;
};