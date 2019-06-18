#include "VoxelRayCast.h"
#include <cmath>
#include "../Engine.h"
#include "../Systems/TerrainGenerationSystem.h"
#include "../Components/BlocksDataComponent.h"
#include <limits>

bool VoxelRayCast::rayCast(XMFLOAT3 origin, XMFLOAT3 direction, float distance, XMINT3 & out_chunk_coord, XMINT3 & out_voxel_coord) {


	XMINT3 origin_chunk(std::floorf(origin.x / static_cast<float>(TERRAIN_CHUNK_DIM)),
		std::floorf(origin.y / static_cast<float>(TERRAIN_CHUNK_DIM)),
		std::floorf(origin.z / static_cast<float>(TERRAIN_CHUNK_DIM)));

	XMINT3 inside_chunk_coord(static_cast<int>(origin.x) % TERRAIN_CHUNK_DIM,
		static_cast<int>(origin.y) % TERRAIN_CHUNK_DIM,
		static_cast<int>(origin.z) % TERRAIN_CHUNK_DIM);


	TerrainGenerationSystem * terrain_system = ENGINE.getSystemManager().getSystem<TerrainGenerationSystem>();

	EntityId chunk_id = terrain_system->getTerrainChunk(origin_chunk);

	BlocksDataComponent *blocks_data = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(chunk_id);
	//TerrainChunk * chunk = ENGINE.getEntityManager().getEntity<TerrainChunk>(chunk_id);

	int step_x = 0;
	if (direction.x > 0) {
		step_x = 1;
	}
	else if (direction.x < 0) {
		step_x = -1;
	}

	int step_y = 0;
	if (direction.y > 0) {
		step_y = 1;
	}
	else if (direction.y < 0) {
		step_y = -1;
	}

	int step_z = 0;
	if (direction.z > 0) {
		step_z = 1;
	}
	else if (direction.z < 0) {
		step_z = -1;
	}

	float current_distance = 0.0f;
	XMINT3 current_voxel_coord = inside_chunk_coord;
	XMINT3 current_chunk_coord = origin_chunk;


	/*float max_x_dist = std::numeric_limits<float>::max();
	float max_x_dist2 = std::numeric_limits<float>::max();
	float delta_dist_x = std::numeric_limits<float>::max();
	if (direction.x != 0.0f) {
		max_x_dist = ((origin_chunk.x + step_x) * static_cast<float>(TERRAIN_CHUNK_DIM) - origin.x) / direction.x;
		max_x_dist2 = ((origin_chunk.x + step_x + step_x) * static_cast<float>(TERRAIN_CHUNK_DIM) - (origin_chunk.x + step_x)) / direction.x;
		delta_dist_x = max_x_dist2 - max_x_dist;
	}
	float max_y_dist = std::numeric_limits<float>::max();
	float max_y_dist2 = std::numeric_limits<float>::max();
	float delta_dist_y = std::numeric_limits<float>::max();;
	if (direction.y != 0.0f) {
		max_y_dist = ((origin_chunk.y + step_y) * static_cast<float>(TERRAIN_CHUNK_DIM) - origin.y) / direction.y;
		max_y_dist2 = ((origin_chunk.y + step_y + step_y) * static_cast<float>(TERRAIN_CHUNK_DIM) - (origin_chunk.y + step_y)) / direction.y;
		delta_dist_y = max_y_dist2 - max_y_dist;
	}
	float max_z_dist = std::numeric_limits<float>::max();
	float max_z_dist2 = std::numeric_limits<float>::max();
	float delta_dist_z = std::numeric_limits<float>::max();
	if (direction.z != 0.0f) {
		max_z_dist = ((origin_chunk.z + step_z) * static_cast<float>(TERRAIN_CHUNK_DIM) - origin.z) / direction.z;
		max_z_dist2 = ((origin_chunk.z + step_z + step_z) * static_cast<float>(TERRAIN_CHUNK_DIM) - (origin_chunk.z + step_z)) / direction.z;
		delta_dist_z = max_z_dist2 - max_z_dist;
	}*/


	float max_x_dist = std::numeric_limits<float>::max();
	float max_x_dist2 = std::numeric_limits<float>::max();
	float delta_dist_x = std::numeric_limits<float>::max();
	if (direction.x != 0.0f) {
		max_x_dist = (((origin_chunk.x * static_cast<float>(TERRAIN_CHUNK_DIM) ) + static_cast<float>(inside_chunk_coord.x + step_x)) - origin.x) / direction.x;
		max_x_dist2 = (((origin_chunk.x * static_cast<float>(TERRAIN_CHUNK_DIM)) + static_cast<float>(inside_chunk_coord.x + step_x + step_x)) - origin.x) / direction.x;
		delta_dist_x = max_x_dist2 - max_x_dist;
	}
	float max_y_dist = std::numeric_limits<float>::max();
	float max_y_dist2 = std::numeric_limits<float>::max();
	float delta_dist_y = std::numeric_limits<float>::max();
	if (direction.y != 0.0f) {
		max_y_dist = (((origin_chunk.y * static_cast<float>(TERRAIN_CHUNK_DIM)) + static_cast<float>(inside_chunk_coord.y + step_y)) - origin.y) / direction.y;
		max_y_dist2 = (((origin_chunk.y * static_cast<float>(TERRAIN_CHUNK_DIM)) + static_cast<float>(inside_chunk_coord.y + step_y + step_y)) - origin.y) / direction.y;
		delta_dist_y = max_y_dist2 - max_y_dist;
	}
	float max_z_dist = std::numeric_limits<float>::max();
	float max_z_dist2 = std::numeric_limits<float>::max();
	float delta_dist_z = std::numeric_limits<float>::max();
	if (direction.z != 0.0f) {
		max_z_dist = (((origin_chunk.z * static_cast<float>(TERRAIN_CHUNK_DIM)) + static_cast<float>(inside_chunk_coord.z + step_z)) - origin.z) / direction.z;
		max_z_dist2 = (((origin_chunk.z * static_cast<float>(TERRAIN_CHUNK_DIM)) + static_cast<float>(inside_chunk_coord.z + step_z + step_z)) - origin.z) / direction.z;
		delta_dist_z = max_z_dist2 - max_z_dist;
	}

	bool found_flag = false;
	while (current_distance <= distance) {
		char s[256];
		sprintf(s, "Ray cast chunk: %u, %u, %u\n", current_chunk_coord.x, current_chunk_coord.y, current_chunk_coord.z);
		OutputDebugStringA(s);

		EntityId chunk_id = terrain_system->getTerrainChunk(current_chunk_coord);

		BlocksDataComponent * blocks_data = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(chunk_id);
		if (blocks_data != nullptr) {
			if (blocks_data->getBlock(current_voxel_coord) != BlockType::AIR) {
				out_chunk_coord = current_chunk_coord;
				out_voxel_coord = current_voxel_coord;
				/*out_coord = XMINT3((current_chunk_coord.x * TERRAIN_CHUNK_DIM) + current_voxel_coord.x,
										(current_chunk_coord.y * TERRAIN_CHUNK_DIM) + current_voxel_coord.y,
										(current_chunk_coord.z * TERRAIN_CHUNK_DIM) + current_voxel_coord.z);*/
				found_flag = true;
				break;
			}

		}

		if (max_x_dist <= max_y_dist && max_x_dist <= max_z_dist) {
			current_distance += max_x_dist;
			max_x_dist += delta_dist_x;
			current_voxel_coord.x += step_x;
			if (current_voxel_coord.x == TERRAIN_CHUNK_DIM) {
				current_chunk_coord.x += step_x;
				current_voxel_coord.x = 0;
			}
			else if (current_voxel_coord.x == -1) {
				current_chunk_coord.x += step_x;
				current_voxel_coord.x = TERRAIN_CHUNK_DIM - 1;
			}
		}
		else if (max_y_dist <= max_x_dist && max_y_dist <= max_z_dist) {
			current_distance += max_y_dist;
			max_y_dist += delta_dist_y;
			current_voxel_coord.y += step_y;
			if (current_voxel_coord.y == TERRAIN_CHUNK_DIM) {
				current_chunk_coord.y += step_y;
				current_voxel_coord.y = 0;
			}
			else if (current_voxel_coord.y == -1) {
				current_chunk_coord.y += step_y;
				current_voxel_coord.y = TERRAIN_CHUNK_DIM - 1;
			}
		}
		else if (max_z_dist <= max_x_dist && max_z_dist <= max_y_dist) {
			current_distance += max_z_dist;
			max_z_dist += delta_dist_z;
			current_voxel_coord.z += step_z;
			if (current_voxel_coord.z == TERRAIN_CHUNK_DIM) {
				current_chunk_coord.z += step_x;
				current_voxel_coord.z = 0;
			}
			else if (current_voxel_coord.z == -1) {
				current_chunk_coord.z += step_z;
				current_voxel_coord.z = TERRAIN_CHUNK_DIM - 1;
			}
		}


	}


	return found_flag;
}
