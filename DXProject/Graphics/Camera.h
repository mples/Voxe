#pragma once
#include <DirectXMath.h>

#include "GameObject.h"

using namespace DirectX;

class Camera : public GameObject{
public:
	Camera();
	~Camera();

	const XMMATRIX& getViewMatrix() const;
	const XMMATRIX& getProjMatrix() const;
	const XMMATRIX getViewProjMatrix();
	
	void setProjData(float fov_degrees, float aspect_ratio, float near_plane, float far_plane);

	void setLookAt(XMFLOAT3 look_at);
private:
	void updateMatrix() override;

	XMMATRIX viewMatrix_;
	XMMATRIX projMatrix_;
};

