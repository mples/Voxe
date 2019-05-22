#pragma once
#include "../ECS/Component.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/Vertex.h"

class MeshComponent : public Component<MeshComponent> {
public:
	MeshComponent(ID3D11Device * device, std::vector<Vertex>& vertices, std::vector<DWORD>& indices);
	~MeshComponent();

	VertexBuffer<Vertex>& getVertexBuffer();
	IndexBuffer& getIndexBuffer();

private:
	VertexBuffer<Vertex> vertexBuffer_;
	IndexBuffer indexBuffer_;
	UINT offset_;
};