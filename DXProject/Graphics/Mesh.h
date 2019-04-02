#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"

class Mesh {
public:
	Mesh();
	Mesh(ID3D11Device * device, ID3D11DeviceContext * device_context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture> & textures, const DirectX::XMMATRIX & transform);
	Mesh(const Mesh& other);
	~Mesh();
	void draw();
	const DirectX::XMMATRIX & getTransform();
private:
	ID3D11DeviceContext * deviceContext_;
	VertexBuffer<Vertex> vertexBuffer_;
	IndexBuffer indexBuffer_;
	std::vector<Texture> textures_;
	DirectX::XMMATRIX transformMatrix_;
};

