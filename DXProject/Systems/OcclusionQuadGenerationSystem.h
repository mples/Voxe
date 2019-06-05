#pragma once

#include <d3d11.h>
#include "../ECS/System.h"

#include "../Components/OcclusionQuadsComponent.h"
#include "../Components/BlocksDataComponent.h"
#include "../Events/DirectXDeviceCreated.h"
#include "../Events/VoxelDataGeneratedEvent.h"
#include "../ECS/Event/IEventListener.h"

#include "../Utilities/Structures/Array2D.h"
#include <list>
#include <queue>

class OcclusionQuadGenerationSystem : public System<OcclusionQuadGenerationSystem>, public IEventListener {
public:
	OcclusionQuadGenerationSystem();
	~OcclusionQuadGenerationSystem();

	virtual void preUpdate(float dt) override;

	virtual void update(float dt) override;

	virtual void postUpdate(float dt) override;
private:
	void onDirectXDeviceCreated(const DirectXDeviceCreated * e);

	bool generateQuads(EntityId eid);

	void onVoxelDataGeneratedEvent(const VoxelDataGeneratedEvent* e);

	std::vector<std::pair<int, int>> getQuadCorners(Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> table, int invalid_value);
	bool pointWithinBounds(std::pair<int, int> p);
	bool pointWithinBounds(int x, int y);
	void addAdjectPointsFirstNotFound(int x, int y, std::queue<std::pair<int, int>> & point_queue, Array2D<bool,TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & visited_map);
	void addAdjectPointsFirstNotFound(std::pair<int, int> p, std::queue<std::pair<int, int>> & point_queue, Array2D<bool, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & visited_map);

	void addAdjectPointsFirstFound(int x, int y, std::queue<std::pair<int, int>> & point_queue, Array2D<bool, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & visited_map, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & table, int invalid_value);
	void addAdjectPointsFirstFound(std::pair<int, int> p, std::queue<std::pair<int, int>> & point_queue, Array2D<bool, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & visited_map, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & table, int invalid_value);


	void makeQuadsXAxis(Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> max_table, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> min_table, std::vector<Vertex> & out_vertices_max, std::vector<Vertex> & out_vertices_min);
	void makeQuadsYAxis(Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> max_table, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> min_table, std::vector<Vertex> & out_vertices_max, std::vector<Vertex> & out_vertices_min);
	void makeQuadsZAxis(Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> max_table, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> min_table, std::vector<Vertex> & out_vertices_max, std::vector<Vertex> & out_vertices_min);

	int pointDistanceSqare(std::pair<int, int> p1, std::pair<int, int> p2);

	std::list<EntityId> generateList_;

	ID3D11Device * device_;

};