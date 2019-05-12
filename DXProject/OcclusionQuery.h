#pragma once
#include <d3d11.h>

#include "Voxel/Chunk.h"

class OcclusionQuery {
public:
	OcclusionQuery(Chunk * chunk, ID3D11Query * query);
	~OcclusionQuery();

	bool operator==(const OcclusionQuery & other);
	bool getDataIfAvailable(ID3D11DeviceContext * device_context, UINT64 * out_data);
	UINT64 getData(ID3D11DeviceContext * device_context);

	Chunk * getChunk();
private:
	ID3D11Query * query_;
	Chunk * chunk_;
};

