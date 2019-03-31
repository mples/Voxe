#pragma once
#include "GameObject.h"

class RenderableObject : public GameObject {
public:
	RenderableObject();
	~RenderableObject();
	bool initialize(const std::string & file_path, ID3D11Device* device, ID3D11DeviceContext* device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader);
	void draw(const XMMATRIX & view_proj_matrix);
protected:
	void updateMatrix() override;

	Model model_;

	XMMATRIX modelMatrix_;
};

