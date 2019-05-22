#pragma once
#include <DirectXMath.h>

#include "../ECS/Component.h"

using namespace DirectX;

class TransformComponent : public Component<TransformComponent> {
public:
	TransformComponent();
	TransformComponent(float x, float y, float z, float pitch, float yaw, float roll);
	TransformComponent(XMFLOAT3 pos, XMFLOAT3 rot);
	~TransformComponent();

	XMMATRIX& getWorldMatrix();

	const XMFLOAT3& getPos();
	const XMVECTOR& getPosVector();

	const XMFLOAT3& getRot();
	const XMVECTOR& getRotVector();

	const XMVECTOR& getForwardVector();
	const XMVECTOR& getBackwardVector();
	const XMVECTOR& getLeftVector();
	const XMVECTOR& getRightVector();

	void setPos(const XMVECTOR& pos);
	void setPos(const XMFLOAT3& pos);
	void setPos(float x, float y, float z);
	void adjustPos(const XMVECTOR& pos);
	void adjustPos(const XMFLOAT3& pos);
	void adjustPos(float x, float y, float z);

	void setRot(const XMVECTOR& rot);
	void setRot(const XMFLOAT3& rot);
	void setRot(float x, float y, float z);
	void adjustRot(const XMVECTOR& rot);
	void adjustRot(const XMFLOAT3& rot);
	void adjustRot(float x, float y, float z);

protected:
	virtual void updateMatrix();
	static const XMVECTOR DEFAULT_FRONT_VEC;
	static const XMVECTOR DEFAULT_UP_VEC;
	static const XMVECTOR DEFAULT_BACKWARD_VEC;
	static const XMVECTOR DEFAULT_RIGHT_VEC;
	static const XMVECTOR DEFAULT_LEFT_VEC;

	XMFLOAT3 pos_;
	XMVECTOR posVector_;

	XMFLOAT3 rot_;
	XMVECTOR rotVector_;

	XMVECTOR forwardVector_;
	XMVECTOR backwardVector_;
	XMVECTOR leftVector_;
	XMVECTOR rightVector_;

	XMMATRIX worldMatrix_;
};
