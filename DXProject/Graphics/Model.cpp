#include "Model.h"
#include "../Utilities/StringHelper.h"
#include <cassert>

Model::Model() : device_(nullptr), deviceContext_(nullptr), CBVertexShader_(nullptr), directory_{} {
}


Model::~Model() {
}

bool Model::initialize(const std::string & file_path, ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader) {
	device_ = device;
	deviceContext_ = device_context;
	CBVertexShader_ = &cb_vertex_shader;
	try {
		if (!loadModel(file_path)) {
			return false;
		}
	}
	catch (COMException ex) {
		ErrorLogger::log(ex);
		return false;
	}
	return true;
}

void Model::draw(const XMMATRIX & model_matrix, const XMMATRIX & view_proj_matrix) {

	deviceContext_->VSSetConstantBuffers(0, 1, CBVertexShader_->getAddressOf());

	for (auto mesh : meshes_) {
		CBVertexShader_->data_.mvpMatrix_ = mesh.getTransform() * model_matrix * view_proj_matrix;
		CBVertexShader_->data_.modelMatrix_ = mesh.getTransform() * model_matrix;
		CBVertexShader_->applyChanges();
		mesh.draw();
	}

}

bool Model::loadModel(const std::string & file_path) {
	directory_ = StringHelper::stringToWide(StringHelper::getDirectoryFromPath(file_path));
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(file_path , aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	 
	if (scene == nullptr) {
		return false;
	}

	processNode(scene->mRootNode, scene, XMMatrixIdentity());
	return true;
}

void Model::processNode(aiNode * node, const aiScene * scene, const XMMATRIX & parent_transform) {
	XMMATRIX node_transforn = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parent_transform;

	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(processMesh(mesh, scene, node_transforn));
	}
	for (UINT i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, node_transforn);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene, const XMMATRIX & transform) {
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		vertex.pos_.x = mesh->mVertices[i].x;
		vertex.pos_.y = mesh->mVertices[i].y;
		vertex.pos_.z = mesh->mVertices[i].z;

		vertex.normal_.x = mesh->mNormals[i].x;
		vertex.normal_.y = mesh->mNormals[i].y;
		vertex.normal_.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord_.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord_.y = (float)mesh->mTextureCoords[0][i].y;
		}
		vertices.push_back(vertex);
	}

	for (UINT i = 0 ; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0 ; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> textures = loadTexturesFromMaterial(material, aiTextureType_DIFFUSE, scene);
	return Mesh(device_, deviceContext_, vertices, indices, textures, transform);
}

std::vector<Texture> Model::loadTexturesFromMaterial(aiMaterial * material, aiTextureType type, const aiScene * scene) {
	std::vector<Texture> textures;
	TextureStorageType storage_type = TextureStorageType::INVALID;
	UINT texture_count = material->GetTextureCount(type);

	if (texture_count == 0) {
		storage_type = TextureStorageType::NONE;
		aiColor3D ai_color(0.0f, 0.0f, 0.0f);
		switch (type) {
		case aiTextureType_DIFFUSE:
			material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_color);
			if (ai_color.IsBlack()) {
				textures.push_back(Texture(device_, Colors::UnloadedColor, type));
				return textures;
			}
			else {
				textures.push_back(Texture(device_, Color(ai_color.r * 255, ai_color.g * 255, ai_color.b * 255), type));
				return textures;
			}
		}
	}
	else {
		for (UINT i = 0; i < texture_count; i++) {
			aiString path;
			material->GetTexture(type, i, &path);
			TextureStorageType storage_type = determineTextureStorageType(scene, material, i, type);
			switch (storage_type) {
			case TextureStorageType::EMBEDDED_INDEX_COMPRESSED: {
				int index = getTextureIndex(&path);
				textures.push_back(Texture(device_, reinterpret_cast<uint8_t*>(scene->mTextures[index]->pcData), scene->mTextures[index]->mWidth, type));
				break;
			}
			case TextureStorageType::EMBEDDED_COMPRESSED: {
				const aiTexture * tex = scene->GetEmbeddedTexture(path.C_Str());
				textures.push_back(Texture(device_, reinterpret_cast<uint8_t*>(tex->pcData), tex->mWidth, type));
				break;
			}

			case TextureStorageType::DISK: {
				std::wstring filename = directory_ + L"/" + StringHelper::stringToWide(std::string(path.C_Str()));
				textures.push_back(Texture(device_, filename, type));
				break;
			}

			}
		}
	}
	if (textures.empty()) {
		textures.push_back(Texture(device_, Colors::UnhandledColor, type));
	}
	return textures;

}

TextureStorageType Model::determineTextureStorageType(const aiScene * scene, aiMaterial * material, UINT index, aiTextureType type) {
	assert( 0 != material->GetTextureCount(type) );

	aiString path;
	material->GetTexture(type, index, &path);
	std::string texture_path = path.C_Str();

	if (texture_path[0] == '*') { //check if embedded indexed texture
		if (scene->mTextures[0]->mHeight == 0) {
			return TextureStorageType::EMBEDDED_INDEX_COMPRESSED;
		}
		else {
			assert("NON COMPRESSED TEXTURES ARE NOT SUPPORTED" && 0);
			return TextureStorageType::EMBEDDED_INDEX_NON_COMPRESSED;
		}
	}
	if (auto tex = scene->GetEmbeddedTexture(texture_path.c_str())) {
		if (tex->mHeight == 0) {
			return TextureStorageType::EMBEDDED_COMPRESSED;
		}
		else {
			assert("NON COMPRESSED TEXTURES ARE NOT SUPPORTED" && 0);
			return TextureStorageType::EMBEDDED_NON_COMPRESSED;
		}
	}
	if (texture_path.find(".") != std::string::npos) {
		return TextureStorageType::DISK;
	}
	return TextureStorageType::NONE;
}

int Model::getTextureIndex(aiString * str) {
	assert(str->length >= 2);

	return atoi(&str->C_Str()[1]);
}
