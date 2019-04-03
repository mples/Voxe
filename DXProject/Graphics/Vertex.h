#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Vertex {
	Vertex() = default;
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : pos_(x, y, z), texCoord_(u, v), normal_(nx, ny, nz) {}
	Vertex(XMFLOAT3 pos, XMFLOAT2 tex_coord, XMFLOAT3 normal) : pos_(pos), texCoord_(tex_coord), normal_(normal) {}

	DirectX::XMFLOAT3 pos_;
	DirectX::XMFLOAT2 texCoord_;
	DirectX::XMFLOAT3 normal_;
};
