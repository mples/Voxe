#pragma once
#include "Model.h"

class GameObject {
public:
	GameObject();
	~GameObject();

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
	XMFLOAT3 pos_;
	XMVECTOR posVector_;

	XMFLOAT3 rot_;
	XMVECTOR rotVector_;

	const XMVECTOR DEFAULT_FRONT_VEC = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VEC = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VEC = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VEC = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VEC = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR forwardVector_;
	XMVECTOR backwardVector_;
	XMVECTOR leftVector_;
	XMVECTOR rightVector_;
};

