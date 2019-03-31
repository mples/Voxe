#pragma once
#include "Mesh.h"

using namespace DirectX;
class Model {
public:
	Model();
	~Model();
	bool initialize(const std::string & file_path, ID3D11Device* device, ID3D11DeviceContext* device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader);
	void draw(const XMMATRIX & model_matrix, const XMMATRIX & view_proj_matrix);

private:
	bool loadModel(const std::string & file_path);
	void processNode(aiNode * node, const aiScene * scene, const XMMATRIX & parent_transform);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene, const XMMATRIX & transform);
	std::vector<Texture> loadTexturesFromMaterial(aiMaterial * material, aiTextureType type, const aiScene * scene);
	TextureStorageType determineTextureStorageType(const aiScene * scene, aiMaterial * material, UINT index, aiTextureType type);
	int getTextureIndex(aiString * str);

	std::vector<Mesh> meshes_;
	std::wstring directory_;

	ID3D11Device* device_;
	ID3D11DeviceContext* deviceContext_;
	ConstantBuffer<CB_VS_object_buffer> * CBVertexShader_;
};

