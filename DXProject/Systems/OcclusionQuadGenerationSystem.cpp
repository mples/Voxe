#include "OcclusionQuadGenerationSystem.h"
#include "../Engine.h"
#include <queue>
#include <unordered_set>

OcclusionQuadGenerationSystem::OcclusionQuadGenerationSystem(): IEventListener(ENGINE.getEventHandler()), device_(nullptr) {
	registerEventCallback<DirectXDeviceCreated>([&](const DirectXDeviceCreated * e) {
		onDirectXDeviceCreated(e);
	});
	registerEventCallback<VoxelDataGeneratedEvent>([&](const VoxelDataGeneratedEvent * e) {
		onVoxelDataGeneratedEvent(e);
	});
}

OcclusionQuadGenerationSystem::~OcclusionQuadGenerationSystem() {
}

void OcclusionQuadGenerationSystem::preUpdate(float dt) {
}

void OcclusionQuadGenerationSystem::update(float dt) {
	if (device_ == nullptr) {
		return;
	}

	for (EntityId& id : generateList_) {
		generateQuads(id);
	}
	generateList_.clear();
}

void OcclusionQuadGenerationSystem::postUpdate(float dt) {
}

void OcclusionQuadGenerationSystem::onDirectXDeviceCreated(const DirectXDeviceCreated * e) {
	if (device_ == nullptr) {
		device_ = e->device_;
	}
}

bool OcclusionQuadGenerationSystem::generateQuads(EntityId eid) {
	BlocksDataComponent * block_data = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(eid);
	if (block_data == nullptr || block_data->isEmpty()) {
		return false;
	}
	
	Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM>  x_max;

	Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> x_min;

	Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> y_max;
	Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> y_min;

	Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> z_max;
	Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> z_min;

	for (int i = 0; i < TERRAIN_CHUNK_DIM; i++) {
		for (int j = 0; j < TERRAIN_CHUNK_DIM; j++) {
			x_max.at(i, j) = -1;
			x_min.at(i, j) = TERRAIN_CHUNK_DIM;

			y_max.at(i,j) = -1;
			y_min.at(i, j) = TERRAIN_CHUNK_DIM;

			z_max.at(i, j) = -1;
			z_min.at(i,j) = TERRAIN_CHUNK_DIM;
		}
	}

	for (int x = 0; x < TERRAIN_CHUNK_DIM; x++) {
		for (int y = 0; y < TERRAIN_CHUNK_DIM; y++) {
			for (int z = 0; z < TERRAIN_CHUNK_DIM; z++) {
				if (block_data->getBlock(x, y, z) != BlockType::AIR) {
					if (x_max.at(y, z) < x) {
						x_max.at(y, z) = x;
					}
					if (x_min.at(y, z) > x) {
						x_min.at(y, z) = x;
					}

					if (y_max.at(x, z) < y) {
						y_max.at(x, z) = y;
					}
					if (y_min.at(x, z) > y) {
						y_min.at(x, z) = y;
					}

					if (z_max.at(x, y) < z) {
						z_max.at(x, y) = z;
					}
					if (z_min.at(x, y) > z) {
						z_min.at(x, y) = z;
					}
				}
			}
		}
	}

	
	OcclusionQuadsComponent * occl_quads = ENGINE.getComponentManager().addComponent<OcclusionQuadsComponent>(eid);
	std::vector<Vertex> vertices_max;
	std::vector<Vertex> vertices_min;
	std::vector<DWORD> indices_max;
	std::vector<DWORD> indices_min;

	indices_max = { 0, 1, 2, 0, 2, 3 };
	indices_min = { 0, 3, 2, 0, 2, 1 };
	//x
	std::vector<std::pair<int,int>> corners = getQuadCorners(x_max, -1);

	vertices_max = {
		Vertex(XMFLOAT3(
			static_cast<float>(1 + x_max.at(corners[0].first, corners[0].second)), //bl
			static_cast<float>(corners[0].first),
			static_cast<float>(corners[0].second)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(1 + x_max.at(corners[1].first, corners[1].second)), //tl
			static_cast<float>(corners[1].first + 1),
			static_cast<float>(corners[1].second)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(1 + x_max.at(corners[2].first, corners[2].second)), //tr
			static_cast<float>(corners[2].first + 1),
			static_cast<float>(corners[2].second + 1)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(1 + x_max.at(corners[3].first, corners[3].second)), //br
			static_cast<float>(corners[3].first),
			static_cast<float>(corners[3].second + 1)
		)),

	};

	occl_quads->positiveX_.vertexBuffer_.initialize(device_, vertices_max.data(), vertices_max.size());
	occl_quads->positiveX_.indexBuffer_.initialize(device_, indices_max.data(), indices_max.size());
	occl_quads->positiveX_.offset_ = 0;

	corners.clear();
	corners = getQuadCorners(x_min, TERRAIN_CHUNK_DIM);

	vertices_min = {
		Vertex(XMFLOAT3(
			static_cast<float>(x_min.at(corners[0].first, corners[0].second)), //bl
			static_cast<float>(corners[0].first),
			static_cast<float>(corners[0].second)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(x_min.at(corners[1].first, corners[1].second)), //tl
			static_cast<float>(corners[1].first + 1),
			static_cast<float>(corners[1].second)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(x_min.at(corners[2].first, corners[2].second)), //tr
			static_cast<float>(corners[2].first + 1),
			static_cast<float>(corners[2].second + 1)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(x_min.at(corners[3].first, corners[3].second)), //br
			static_cast<float>(corners[3].first),
			static_cast<float>(corners[3].second + 1)
		)),

	};

	occl_quads->negativeX_.vertexBuffer_.initialize(device_, vertices_min.data(), vertices_min.size());
	occl_quads->negativeX_.indexBuffer_.initialize(device_, indices_min.data(), indices_min.size());
	occl_quads->negativeX_.offset_ = 0;

	vertices_max.clear();
	vertices_min.clear();
	indices_max.clear();
	indices_min.clear();

	//y
	corners.clear();
	corners = getQuadCorners(y_max, -1);

	vertices_max = {
		Vertex(XMFLOAT3(
			static_cast<float>(corners[0].first),
			static_cast<float>(1 + y_max.at(corners[0].first, corners[0].second)), //bl
			static_cast<float>(corners[0].second)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[1].first),
			static_cast<float>(1 + y_max.at(corners[1].first, corners[1].second)), //tl
			static_cast<float>(corners[0].second + 1)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[2].first + 1),
			static_cast<float>(1 + y_max.at(corners[2].first, corners[2].second)), //tr
			static_cast<float>(corners[2].second + 1)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[3].first + 1),
			static_cast<float>(1 + y_max.at(corners[3].first, corners[3].second)), //br
			static_cast<float>(corners[3].second)
		)),

	};

	occl_quads->positiveY_.vertexBuffer_.initialize(device_, vertices_max.data(), vertices_max.size());
	occl_quads->positiveY_.indexBuffer_.initialize(device_, indices_max.data(), indices_max.size());
	occl_quads->positiveY_.offset_ = 0;

	corners.clear();
	corners = getQuadCorners(y_min, TERRAIN_CHUNK_DIM);

	vertices_min = {
		Vertex(XMFLOAT3(
			static_cast<float>(corners[0].first),
			static_cast<float>(y_min.at(corners[0].first, corners[0].second)), //bl
			static_cast<float>(corners[0].second)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[1].first),
			static_cast<float>(y_min.at(corners[1].first, corners[1].second)), //tl
			static_cast<float>(corners[1].second + 1)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[2].first + 1),
			static_cast<float>(y_min.at(corners[2].first, corners[2].second)), //tr
			static_cast<float>(corners[2].second + 1)
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[3].first + 1),
			static_cast<float>(y_min.at(corners[3].first, corners[3].second)), //br
			static_cast<float>(corners[3].second)
		)),

	};

	occl_quads->negativeY_.vertexBuffer_.initialize(device_, vertices_min.data(), vertices_min.size());
	occl_quads->negativeY_.indexBuffer_.initialize(device_, indices_min.data(), indices_min.size());
	occl_quads->negativeY_.offset_ = 0;

	vertices_max.clear();
	vertices_min.clear();
	indices_max.clear();
	indices_min.clear();

	//z
	corners.clear();
	corners = getQuadCorners(z_max, -1);
	for (std::pair<int, int> p : corners) {
		vertices_max.push_back(Vertex(XMFLOAT3(
			static_cast<float>(p.first),
			static_cast<float>(p.second),
			static_cast<float>(z_max.at(p.first, p.second))
		)));
	}

	vertices_max = {
		Vertex(XMFLOAT3(
			static_cast<float>(corners[0].first),
			static_cast<float>(corners[0].second),
			static_cast<float>(1 + z_max.at(corners[0].first, corners[0].second)) //bl
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[1].first),
			static_cast<float>(corners[1].second + 1),
			static_cast<float>(1 + z_max.at(corners[1].first, corners[1].second)) //tl
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[2].first + 1),
			static_cast<float>(corners[2].second + 1),
			static_cast<float>(1 + z_max.at(corners[2].first, corners[2].second)) //tr
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[3].first + 1),
			static_cast<float>(corners[3].second),
			static_cast<float>(1 + z_max.at(corners[3].first, corners[3].second)) //br
		)),

	};

	occl_quads->positiveZ_.vertexBuffer_.initialize(device_, vertices_max.data(), vertices_max.size());
	occl_quads->positiveZ_.indexBuffer_.initialize(device_, indices_max.data(), indices_max.size());
	occl_quads->positiveZ_.offset_ = 0;

	corners.clear();
	corners = getQuadCorners(z_min, TERRAIN_CHUNK_DIM);

	vertices_min = {
		Vertex(XMFLOAT3(
			static_cast<float>(corners[0].first),
			static_cast<float>(corners[0].second),
			static_cast<float>(z_min.at(corners[0].first, corners[0].second)) //bl
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[1].first),
			static_cast<float>(corners[1].second + 1),
			static_cast<float>(z_min.at(corners[1].first, corners[1].second)) //tl
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[2].first + 1),
			static_cast<float>(corners[2].second + 1),
			static_cast<float>(z_min.at(corners[2].first, corners[2].second)) //tr
		)),
		Vertex(XMFLOAT3(
			static_cast<float>(corners[3].first + 1),
			static_cast<float>(corners[3].second),
			static_cast<float>(z_min.at(corners[3].first, corners[3].second)) //br
		)),

	};

	occl_quads->negativeZ_.vertexBuffer_.initialize(device_, vertices_min.data(), vertices_min.size());
	occl_quads->negativeZ_.indexBuffer_.initialize(device_, indices_min.data(), indices_min.size());
	occl_quads->negativeZ_.offset_ = 0;

	vertices_max.clear();
	vertices_min.clear();
	indices_max.clear();
	indices_min.clear();
	
}

void OcclusionQuadGenerationSystem::onVoxelDataGeneratedEvent(const VoxelDataGeneratedEvent * e) {
	generateList_.push_back(e->id_);
}

std::vector<std::pair<int, int>> OcclusionQuadGenerationSystem::getQuadCorners(Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> table, int invalid_value) {
	Array2D<bool, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> visited_map;
	visited_map.fillWith(false);

	std::pair<int, int> bl_quad_corner(-1, -1);
	std::pair<int, int> br_quad_corner(-1, -1);
	std::pair<int, int> tl_quad_corner(-1, -1);
	std::pair<int, int> tr_quad_corner(-1, -1);

	int closest_dist_bl = std::numeric_limits<int>::max();
	int closest_dist_br = std::numeric_limits<int>::max();
	int closest_dist_tl = std::numeric_limits<int>::max();
	int closest_dist_tr = std::numeric_limits<int>::max();

	static std::pair<int, int> bl_corner(0, 0);
	static std::pair<int, int> br_corner(TERRAIN_CHUNK_DIM, 0);
	static std::pair<int, int> tl_corner(0, TERRAIN_CHUNK_DIM);
	static std::pair<int, int> tr_corner(TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM);

	std::queue<std::pair<int, int>> point_queue;
	point_queue.push(std::make_pair(0, 0));
	visited_map.at(0, 0) = true;

	bool found_first_point = false;
	while (!point_queue.empty()) {
		std::pair<int, int> p = point_queue.front();
		point_queue.pop();
		if (!found_first_point) {
			if (table.at(p.first, p.second) != invalid_value) {
				found_first_point = true;

				int distance = pointDistanceSqare(p, bl_corner);
				if (distance < closest_dist_bl) {
					bl_quad_corner = p;
					closest_dist_bl = distance;
				}
				distance = pointDistanceSqare(p, br_corner);
				if (distance < closest_dist_br) {
					br_quad_corner = p;
					closest_dist_br = distance;
				}
				distance = pointDistanceSqare(p, tl_corner);
				if (distance < closest_dist_tl) {
					tl_quad_corner = p;
					closest_dist_tl = distance;
				}
				distance = pointDistanceSqare(p, tr_corner);
				if (distance < closest_dist_tr) {
					tr_quad_corner = p;
					closest_dist_tr = distance;
				}
				addAdjectPointsFirstFound(p, point_queue, visited_map, table, invalid_value);
			}
			else {
				addAdjectPointsFirstNotFound(p, point_queue, visited_map);
			}
		}
		else {
			int distance = pointDistanceSqare(p, bl_corner);
			if (distance < closest_dist_bl) {
				bl_quad_corner = p;
				closest_dist_bl = distance;
			}
			distance = pointDistanceSqare(p, br_corner);
			if (distance < closest_dist_br) {
				br_quad_corner = p;
				closest_dist_br = distance;
			}
			distance = pointDistanceSqare(p, tl_corner);
			if (distance < closest_dist_tl) {
				tl_quad_corner = p;
				closest_dist_tl = distance;
			}
			distance = pointDistanceSqare(p, tr_corner);
			if (distance < closest_dist_tr) {
				tr_quad_corner = p;
				closest_dist_tr = distance;
			}

			addAdjectPointsFirstFound(p, point_queue, visited_map, table, invalid_value);
		}
	}

	return std::vector< std::pair<int, int>> {bl_quad_corner, tl_quad_corner, tr_quad_corner, br_quad_corner};
}

bool OcclusionQuadGenerationSystem::pointWithinBounds(std::pair<int, int> p) {
	return p.first >= 0 && p.first < TERRAIN_CHUNK_DIM && p.second >= 0 && p.second < TERRAIN_CHUNK_DIM;
}

bool OcclusionQuadGenerationSystem::pointWithinBounds(int x, int y) {
	return x >= 0 && x < TERRAIN_CHUNK_DIM && y >= 0 && y < TERRAIN_CHUNK_DIM;
}

void OcclusionQuadGenerationSystem::addAdjectPointsFirstFound(int x, int y, std::queue<std::pair<int, int>>& point_queue, Array2D<bool, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM>& visited_map, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & table, int invalid_value) {
	addAdjectPointsFirstFound(std::make_pair(x, y), point_queue, visited_map, table, invalid_value);
}

void OcclusionQuadGenerationSystem::addAdjectPointsFirstFound(std::pair<int, int> p, std::queue<std::pair<int, int>>& point_queue, Array2D<bool, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM>& visited_map, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & table, int invalid_value) {
	if (pointWithinBounds(p.first + 1, p.second) && visited_map.at(p.first + 1, p.second) == false && table.at(p.first + 1, p.second) != invalid_value) {
		point_queue.push(std::make_pair(p.first + 1, p.second));
		visited_map.at(p.first + 1, p.second) = true;
	}
	if (pointWithinBounds(p.first, p.second + 1) && visited_map.at(p.first, p.second + 1) == false && table.at(p.first, p.second + 1) != invalid_value) {
		point_queue.push(std::make_pair(p.first, p.second + 1));
		visited_map.at(p.first, p.second + 1) = true;
	}
	if (pointWithinBounds(p.first + 1, p.second + 1) && visited_map.at(p.first + 1, p.second + 1) == false && table.at(p.first + 1, p.second + 1) != invalid_value) {
		point_queue.push(std::make_pair(p.first + 1, p.second + 1));
		visited_map.at(p.first + 1, p.second + 1) = true;
	}
	if (pointWithinBounds(p.first - 1, p.second) && visited_map.at(p.first - 1, p.second) == false && table.at(p.first - 1, p.second) != invalid_value) {
		point_queue.push(std::make_pair(p.first - 1, p.second));
		visited_map.at(p.first - 1, p.second) = true;
	}
	if (pointWithinBounds(p.first - 1, p.second - 1) && visited_map.at(p.first - 1, p.second - 1) == false && table.at(p.first - 1, p.second - 1) != invalid_value) {
		point_queue.push(std::make_pair(p.first - 1, p.second - 1));
		visited_map.at(p.first, p.second) = true;
	}
	if (pointWithinBounds(p.first, p.second - 1) && visited_map.at(p.first, p.second - 1) == false && table.at(p.first, p.second - 1) != invalid_value) {
		point_queue.push(std::make_pair(p.first, p.second - 1));
		visited_map.at(p.first, p.second - 1) = true;

	}
	if (pointWithinBounds(p.first + 1, p.second - 1) && visited_map.at(p.first + 1, p.second - 1) == false && table.at(p.first + 1, p.second - 1) != invalid_value) {
		point_queue.push(std::make_pair(p.first + 1, p.second - 1));
		visited_map.at(p.first + 1, p.second - 1) = true;

	}
	if (pointWithinBounds(p.first - 1, p.second + 1) && visited_map.at(p.first - 1, p.second + 1) == false && table.at(p.first - 1, p.second + 1) != invalid_value) {
		point_queue.push(std::make_pair(p.first - 1, p.second + 1));
		visited_map.at(p.first - 1, p.second + 1) = true;
	}
}

void OcclusionQuadGenerationSystem::addAdjectPointsFirstNotFound(int x, int y, std::queue<std::pair<int, int>>& point_queue, Array2D<bool, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM>& visited_map) {
	if (pointWithinBounds(x + 1, y) && visited_map.at(x + 1, y) == false) {
		point_queue.push(std::make_pair(x + 1, y));
		visited_map.at(x + 1,y) = true;
	}
	if (pointWithinBounds(x, y + 1) && visited_map.at(x, y + 1) == false) {
		point_queue.push(std::make_pair(x, y + 1));
		visited_map.at(x, y + 1) = true;
	}
	if (pointWithinBounds(x + 1, y + 1) && visited_map.at(x + 1, y + 1) == false) {
		point_queue.push(std::make_pair(x + 1, y + 1));
		visited_map.at(x + 1, y + 1) = true;
	}
	if (pointWithinBounds(x - 1, y) && visited_map.at(x - 1, y) == false) {
		point_queue.push(std::make_pair(x - 1, y));
		visited_map.at(x - 1, y) = true;
	}
	if (pointWithinBounds(x - 1, y - 1) && visited_map.at(x - 1, y - 1) == false) {
		point_queue.push(std::make_pair(x - 1, y - 1));
		visited_map.at(x - 1, y - 1) = true;
	}
	if (pointWithinBounds(x, y - 1) && visited_map.at(x, y - 1) == false) {
		point_queue.push(std::make_pair(x, y - 1));
		visited_map.at(x, y - 1) = true;
	}
	if (pointWithinBounds(x + 1, y - 1) && visited_map.at(x + 1, y - 1) == false) {
		point_queue.push(std::make_pair(x + 1, y- 1));
		visited_map.at(x + 1, y - 1) = true;
	}
	if (pointWithinBounds(x- 1, y + 1) && visited_map.at(x - 1, y + 1) == false) {
		point_queue.push(std::make_pair(x - 1, y + 1));
		visited_map.at(x - 1, y + 1) = true;
	}
}

void OcclusionQuadGenerationSystem::addAdjectPointsFirstNotFound(std::pair<int, int> p, std::queue<std::pair<int, int>>& point_queue, Array2D<bool, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM>& visited_map) {
	addAdjectPointsFirstNotFound(p.first, p.second, point_queue, visited_map);
}

void OcclusionQuadGenerationSystem::makeQuadsXAxis(Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> max_table, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> min_table, std::vector<Vertex> & out_vertices_max, std::vector<Vertex> & out_vertices_min) {
	
	Array2D<bool, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> visited_map;
	visited_map.fillWith(false);

	std::pair<int, int> bl_quad_corner(-1,-1);
	std::pair<int, int> br_quad_corner(-1, -1);
	std::pair<int, int> tl_quad_corner(-1, -1);
	std::pair<int, int> tr_quad_corner(-1, -1);

	int closest_dist_bl = std::numeric_limits<int>::max();
	int closest_dist_br = std::numeric_limits<int>::max();
	int closest_dist_tl = std::numeric_limits<int>::max();
	int closest_dist_tr = std::numeric_limits<int>::max();

	static std::pair<int, int> bl_corner(0, 0);
	static std::pair<int, int> br_corner(TERRAIN_CHUNK_DIM, 0);
	static std::pair<int, int> tl_corner(0, TERRAIN_CHUNK_DIM);
	static std::pair<int, int> tr_corner(TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM);

	std::queue<std::pair<int, int>> point_queue;
	point_queue.push(std::make_pair(0, 0));

	bool found_first_point = false;
	while (!point_queue.empty()) {
		std::pair<int, int> p = point_queue.front();
		if (!found_first_point) {
			if (max_table.at(p.first, p.second) != -1) {
				found_first_point = true;
			}

			visited_map.at(p.first, p.second) = true;

			if (visited_map.at(p.first + 1, p.second) == false) {
				point_queue.push(std::make_pair(p.first + 1, p.second));
			}
			if (visited_map.at(p.first, p.second + 1) == false) {
				point_queue.push(std::make_pair(p.first, p.second + 1));
			}
			if (visited_map.at(p.first + 1, p.second + 1) == false) {
				point_queue.push(std::make_pair(p.first + 1, p.second + 1));
			}
			if (visited_map.at(p.first - 1, p.second) == false) {
				point_queue.push(std::make_pair(p.first - 1, p.second));
			}
			if (visited_map.at(p.first - 1, p.second - 1) == false) {
				point_queue.push(std::make_pair(p.first - 1, p.second - 1));
			}
			if (visited_map.at(p.first, p.second - 1) == false) {
				point_queue.push(std::make_pair(p.first, p.second - 1));
			}
			if (visited_map.at(p.first + 1, p.second - 1) == false) {
				point_queue.push(std::make_pair(p.first + 1, p.second - 1));
			}
			if (visited_map.at(p.first - 1, p.second + 1) == false) {
				point_queue.push(std::make_pair(p.first - 1, p.second + 1));
			}
		}
		else {
			if (pointDistanceSqare(p, bl_corner) < closest_dist_bl) {
				bl_quad_corner = p;
			}
			if (pointDistanceSqare(p, br_corner) < closest_dist_br) {
				br_quad_corner = p;
			}
			if (pointDistanceSqare(p, tl_corner) < closest_dist_tl) {
				tl_quad_corner = p;
			}
			if (pointDistanceSqare(p, tr_corner) < closest_dist_tr) {
				tr_quad_corner = p;
			}

			visited_map.at(p.first, p.second) = true;

			if (visited_map.at(p.first + 1, p.second) == false && max_table.at(p.first + 1, p.second) != -1) {
				point_queue.push(std::make_pair(p.first + 1, p.second));
			}
			if (visited_map.at(p.first, p.second + 1) == false && max_table.at(p.first, p.second + 1) != -1) {
				point_queue.push(std::make_pair(p.first, p.second + 1));
			}
			if (visited_map.at(p.first + 1, p.second + 1) == false && max_table.at(p.first + 1, p.second + 1) != -1) {
				point_queue.push(std::make_pair(p.first + 1, p.second + 1));
			}
			if (visited_map.at(p.first - 1, p.second) == false && max_table.at(p.first - 1, p.second) != -1) {
				point_queue.push(std::make_pair(p.first - 1, p.second));
			}
			if (visited_map.at(p.first - 1, p.second - 1) == false && max_table.at(p.first - 1, p.second - 1) != -1) {
				point_queue.push(std::make_pair(p.first - 1, p.second - 1));
			}
			if (visited_map.at(p.first, p.second - 1) == false && max_table.at(p.first, p.second - 1) != -1) {
				point_queue.push(std::make_pair(p.first, p.second - 1));
			}
			if (visited_map.at(p.first + 1, p.second - 1) == false && max_table.at(p.first + 1, p.second - 1) != -1) {
				point_queue.push(std::make_pair(p.first + 1, p.second - 1));
			}
			if (visited_map.at(p.first - 1, p.second + 1) == false && max_table.at(p.first - 1, p.second + 1) != -1) {
				point_queue.push(std::make_pair(p.first - 1, p.second + 1));
			}
		}

		out_vertices_max.push_back(Vertex(XMFLOAT3(static_cast<float>(max_table.at(bl_quad_corner.first, bl_quad_corner.second)), 
													static_cast<float>( bl_quad_corner.first), 
													static_cast<float>(bl_quad_corner.second))));

		out_vertices_max.push_back(Vertex(XMFLOAT3(static_cast<float>(max_table.at(tl_quad_corner.first, tl_quad_corner.second)),
													static_cast<float>(tl_quad_corner.first),
													static_cast<float>(tl_quad_corner.second))));

		out_vertices_max.push_back(Vertex(XMFLOAT3(static_cast<float>(max_table.at(tr_quad_corner.first, tr_quad_corner.second)),
													static_cast<float>(tr_quad_corner.first),
													static_cast<float>(tr_quad_corner.second))));

		out_vertices_max.push_back(Vertex(XMFLOAT3(static_cast<float>(max_table.at(tl_quad_corner.first, tl_quad_corner.second)),
													static_cast<float>(tl_quad_corner.first),
													static_cast<float>(tl_quad_corner.second))));
	}
}

void OcclusionQuadGenerationSystem::makeQuadsYAxis(Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> max_table, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> min_table, std::vector<Vertex>& out_vertices_max, std::vector<Vertex>& out_vertices_min) {
	
	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(0, 0));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			int value = max_table.at(point.first, point.second);
			if (max_table.at(point.first, point.second) != -1) {
				out_vertices_max.push_back(Vertex(XMFLOAT3(point.first, max_table.at(point.first, point.second), point.second)));
				break;
			}
			else {
				if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM) && point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second + 1));
					point_queue.push(std::make_pair(point.first + 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}
				else if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second));
				}
				else if (point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}

				point_queue.pop();
			}
		}
	}


	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(0, static_cast<int>(TERRAIN_CHUNK_DIM - 1)));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (max_table.at(point.first, point.second) != -1) {
				out_vertices_max.push_back(Vertex(XMFLOAT3(point.first, max_table.at(point.first, point.second), point.second)));
				break;
			}
			else {
				if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM) && point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first + 1, point.second - 1));
					point_queue.push(std::make_pair(point.first + 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}
				else if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second));
				}
				else if (point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}

				point_queue.pop();
			}
		}
	}

	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(static_cast<int>(TERRAIN_CHUNK_DIM - 1), static_cast<int>(TERRAIN_CHUNK_DIM - 1)));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (max_table.at(point.first, point.second) != -1) {
				out_vertices_max.push_back(Vertex(XMFLOAT3(point.first, max_table.at(point.first, point.second), point.second)));
				break;
			}
			else {
				if (point.first - 1 > 0 && point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second - 1));
					point_queue.push(std::make_pair(point.first - 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}
				else if (point.first - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second));
				}
				else if (point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}

				point_queue.pop();
			}
		}
	}

	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(static_cast<int>(TERRAIN_CHUNK_DIM - 1), 0));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (max_table.at(point.first, point.second) != -1) {
				out_vertices_max.push_back(Vertex(XMFLOAT3(point.first, max_table.at(point.first, point.second), point.second)));
				break;
			}
			else {
				if (point.first - 1 > 0 && point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first - 1, point.second + 1));
					point_queue.push(std::make_pair(point.first - 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}
				else if (point.first - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second));
				}
				else if (point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}

				point_queue.pop();
			}
		}
	}


	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(0, 0));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (min_table.at(point.first, point.second) != -1) {
				out_vertices_min.push_back(Vertex(XMFLOAT3(point.first, min_table.at(point.first, point.second), point.second)));
				break;
			}
			else {
				if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM) && point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second + 1));
					point_queue.push(std::make_pair(point.first + 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}
				else if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second));
				}
				else if (point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}

				point_queue.pop();
			}
		}
	}


	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(0, static_cast<int>(TERRAIN_CHUNK_DIM - 1)));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (min_table.at(point.first, point.second) != -1) {
				out_vertices_min.push_back(Vertex(XMFLOAT3(point.first, min_table.at(point.first, point.second), point.second)));
				break;
			}
			else {
				if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM) && point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first + 1, point.second - 1));
					point_queue.push(std::make_pair(point.first + 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}
				else if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second));
				}
				else if (point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}

				point_queue.pop();
			}
		}
	}

	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(static_cast<int>(TERRAIN_CHUNK_DIM - 1), static_cast<int>(TERRAIN_CHUNK_DIM - 1)));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (min_table.at(point.first, point.second) != -1) {
				out_vertices_min.push_back(Vertex(XMFLOAT3(point.first, min_table.at(point.first, point.second), point.second)));
				break;
			}
			else {
				if (point.first - 1 > 0 && point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second - 1));
					point_queue.push(std::make_pair(point.first - 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}
				else if (point.first - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second));
				}
				else if (point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}

				point_queue.pop();
			}
		}
	}

	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(static_cast<int>(TERRAIN_CHUNK_DIM - 1), 0));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (min_table.at(point.first, point.second) != -1) {
				out_vertices_min.push_back(Vertex(XMFLOAT3(point.first, min_table.at(point.first, point.second), point.second)));
				break;
			}
			else {
				if (point.first - 1 > 0 && point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first - 1, point.second + 1));
					point_queue.push(std::make_pair(point.first - 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}
				else if (point.first - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second));
				}
				else if (point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}

				point_queue.pop();
			}
		}
	}
	
}

void OcclusionQuadGenerationSystem::makeQuadsZAxis(Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> max_table, Array2D<int, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> min_table, std::vector<Vertex>& out_vertices_max, std::vector<Vertex>& out_vertices_min) {
	
	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(0, 0));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (max_table.at(point.first, point.second) != -1) {
				out_vertices_max.push_back(Vertex(XMFLOAT3(point.first, point.second, max_table.at(point.first, point.second))));
				break;
			}
			else {
				if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM) && point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second + 1));
					point_queue.push(std::make_pair(point.first + 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}
				else if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second));
				}
				else if (point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}

				point_queue.pop();
			}
		}
	}


	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(0, static_cast<int>(TERRAIN_CHUNK_DIM - 1)));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (max_table.at(point.first, point.second) != -1) {
				out_vertices_max.push_back(Vertex(XMFLOAT3(point.first, point.second, max_table.at(point.first, point.second))));
				break;
			}
			else {
				if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM) && point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first + 1, point.second - 1));
					point_queue.push(std::make_pair(point.first + 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}
				else if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second));
				}
				else if (point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}

				point_queue.pop();
			}
		}
	}

	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(static_cast<int>(TERRAIN_CHUNK_DIM - 1), static_cast<int>(TERRAIN_CHUNK_DIM - 1)));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (max_table.at(point.first, point.second) != -1) {
				out_vertices_max.push_back(Vertex(XMFLOAT3(point.first, point.second, max_table.at(point.first, point.second))));
				break;
			}
			else {
				if (point.first - 1 > 0 && point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second - 1));
					point_queue.push(std::make_pair(point.first - 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}
				else if (point.first - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second));
				}
				else if (point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}

				point_queue.pop();
			}
		}
	}

	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(static_cast<int>(TERRAIN_CHUNK_DIM - 1), 0));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (max_table.at(point.first, point.second) != -1) {
				out_vertices_max.push_back(Vertex(XMFLOAT3(point.first, point.second, max_table.at(point.first, point.second))));
				break;
			}
			else {
				if (point.first - 1 > 0 && point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first - 1, point.second + 1));
					point_queue.push(std::make_pair(point.first - 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}
				else if (point.first - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second));
				}
				else if (point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}

				point_queue.pop();
			}
		}
	}


	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(0, 0));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (min_table.at(point.first, point.second) != -1) {
				out_vertices_min.push_back(Vertex(XMFLOAT3(point.first, point.second, min_table.at(point.first, point.second))));
				break;
			}
			else {
				if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM) && point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second + 1));
					point_queue.push(std::make_pair(point.first + 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}
				else if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second));
				}
				else if (point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}

				point_queue.pop();
			}
		}
	}


	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(0, static_cast<int>(TERRAIN_CHUNK_DIM - 1)));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (min_table.at(point.first, point.second) != -1) {
				out_vertices_min.push_back(Vertex(XMFLOAT3(point.first, point.second, min_table.at(point.first, point.second))));
				break;
			}
			else {
				if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM) && point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first + 1, point.second - 1));
					point_queue.push(std::make_pair(point.first + 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}
				else if (point.first + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first + 1, point.second));
				}
				else if (point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}

				point_queue.pop();
			}
		}
	}

	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(static_cast<int>(TERRAIN_CHUNK_DIM - 1), static_cast<int>(TERRAIN_CHUNK_DIM - 1)));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (min_table.at(point.first, point.second) != -1) {
				out_vertices_min.push_back(Vertex(XMFLOAT3(point.first, point.second, min_table.at(point.first, point.second))));
				break;
			}
			else {
				if (point.first - 1 > 0 && point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second - 1));
					point_queue.push(std::make_pair(point.first - 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}
				else if (point.first - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second));
				}
				else if (point.second - 1 > 0) {
					point_queue.push(std::make_pair(point.first, point.second - 1));

				}

				point_queue.pop();
			}
		}
	}

	{
		std::queue<std::pair<int, int>> point_queue;
		point_queue.push(std::make_pair(static_cast<int>(TERRAIN_CHUNK_DIM - 1), 0));
		while (!point_queue.empty()) {
			std::pair<int, int> point = point_queue.front();
			if (min_table.at(point.first, point.second) != -1) {
				out_vertices_min.push_back(Vertex(XMFLOAT3(point.first, point.second, min_table.at(point.first, point.second))));
				break;
			}
			else {
				if (point.first - 1 > 0 && point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first - 1, point.second + 1));
					point_queue.push(std::make_pair(point.first - 1, point.second));
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}
				else if (point.first - 1 > 0) {
					point_queue.push(std::make_pair(point.first - 1, point.second));
				}
				else if (point.second + 1 < static_cast<int>(TERRAIN_CHUNK_DIM)) {
					point_queue.push(std::make_pair(point.first, point.second + 1));

				}

				point_queue.pop();
			}
		}
	}
	
}

int OcclusionQuadGenerationSystem::pointDistanceSqare(std::pair<int, int> p1, std::pair<int, int> p2) {
	return std::pow(p1.first - p2.first, 2) + std::pow(p1.second - p2.second, 2);
}
