#pragma once
#include <DirectXMath.h>

struct Vertex {
	Vertex() = default;
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : pos_(x, y, z), texCoord_(u, v), normal_(nx, ny, nz) {}

	DirectX::XMFLOAT3 pos_;
	DirectX::XMFLOAT2 texCoord_;
	DirectX::XMFLOAT3 normal_;
};
