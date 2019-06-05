#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Vertex {
	Vertex() = default;
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : pos_(x, y, z), texCoord_(u, v), normal_(nx, ny, nz) {}
	Vertex(XMFLOAT3 pos, XMFLOAT2 tex_coord, XMFLOAT3 normal) : pos_(pos), texCoord_(tex_coord), normal_(normal) {}
	Vertex(XMFLOAT3 pos) : pos_(pos), texCoord_(0.0f, 0.0f), normal_(0.0f, 0.0f, 0.0f) {}

	DirectX::XMFLOAT3 pos_;
	DirectX::XMFLOAT2 texCoord_;
	DirectX::XMFLOAT3 normal_;
};

struct SimpleVertex {
	SimpleVertex() = default;
	SimpleVertex(float x, float y, float z) : pos_(x, y, z) {}
	SimpleVertex(XMFLOAT3 pos) : pos_(pos) {}

	XMFLOAT3 pos_;
};