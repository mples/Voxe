#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <DirectXMath.h>

#include "../Voxel/BlockType.h"
#include "Color.h"

enum class TextureVertex : unsigned int {
	_00 = 0,
	_01 = 1,
	_10 = 2,
	_11 = 3
};

enum class TextureSide : unsigned int {
	BACK = 5,
	FRONT = 4,
	RIGHT = 3,
	LEFT = 2,
	BOTTOM = 1,
	TOP = 0
};

using namespace DirectX;

class BlocksTextureAtlas {
public:
	BlocksTextureAtlas(ID3D11Device * device, const std::wstring & file_path, std::vector<BlockType> block_types);
	~BlocksTextureAtlas();

	ID3D11ShaderResourceView * getResourceView();
	ID3D11ShaderResourceView ** getResourceViewAddress();

	XMFLOAT2 getTextureCoord(BlockType type, TextureSide side, TextureVertex vertex);
private:
	void initializeColorTexture(ID3D11Device * device, const Color * color_data, UINT width, UINT height);

	Microsoft::WRL::ComPtr<ID3D11Resource> resource_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView_;
	int singleTextureDim_;

	int width_;
	int height_;

	std::unordered_map<BlockType, XMFLOAT2> coordinateMap_;
};