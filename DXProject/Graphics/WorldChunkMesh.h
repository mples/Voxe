#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "Texture.h"


class WorldChunkMesh {
public:
	WorldChunkMesh();
	~WorldChunkMesh();
	bool initialize(ID3D11Device * device, ID3D11DeviceContext * device_context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, Texture* texture);
	void draw();
	void reset();
private:
	ID3D11DeviceContext * deviceContext_;
	VertexBuffer<Vertex> vertexBuffer_;
	IndexBuffer indexBuffer_;
	Texture * texture_;
};

