#pragma once
#include <DirectXCollision.h>
#include <d3d11.h>
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Vertex.h"
#include "Graphics/Texture.h"

using namespace DirectX;

class BoundingVolume {
public:
	BoundingVolume();
	BoundingVolume(XMINT3 coord, BoundingBox bounding_box);
	BoundingVolume(int x, int y, int z, BoundingBox bounding_box);
	~BoundingVolume();

	bool initialize(ID3D11Device * device, ID3D11DeviceContext * device_context);

	void draw();

	BoundingBox & getBoundingVolume();
	XMINT3 & getCoord();
	XMFLOAT3 getPos();

private:
	BoundingBox	boundingBox_;
	XMINT3 coord_;

	ID3D11DeviceContext * deviceContext_;
	VertexBuffer<SimpleVertex> vertexBuffer_;
	IndexBuffer indexBuffer_;
};

