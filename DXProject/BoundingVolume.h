#pragma once
#include <DirectXCollision.h>

using namespace DirectX;

class BoundingVolume {
public:
	BoundingVolume();
	BoundingVolume(XMINT3 coord, BoundingBox bounding_box);
	BoundingVolume(int x, int y, int z, BoundingBox bounding_box);
	~BoundingVolume();

	BoundingBox & getBoundingVolume();
	XMINT3 & getCoord();

private:
	BoundingBox	boundingBox_;
	XMINT3 coord_;
};

