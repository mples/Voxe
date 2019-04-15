#pragma once
#include <DirectXCollision.h>
#include "WorldChunkMesh.h"
#include "ConstantBuffer.h"

class WorldChunkModel {
public:
	WorldChunkModel();
	~WorldChunkModel();

	bool initialize(ID3D11Device* device, ID3D11DeviceContext* device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, Texture * texture);
	void draw(DirectX::XMMATRIX model_matrix, DirectX::XMMATRIX view_proj_matrix);
	void loadData(std::vector<Vertex>& vertices, std::vector<DWORD>& indices);
	BoundingBox & getBoundingBox();
	void reset();
private:
	ID3D11Device* device_;
	ID3D11DeviceContext* deviceContext_;
	ConstantBuffer<CB_VS_object_buffer> * CBVertexShader_;
	WorldChunkMesh mesh_;
	Texture * texture_;
	BoundingBox boundingBox_;
};

