#pragma once
#include <DirectXMath.h>

#include "../ECS/Component.h"

using namespace DirectX;

class WorldCoordinateComponent : public Component<WorldCoordinateComponent> {
public:
	WorldCoordinateComponent();
	WorldCoordinateComponent(int x, int y, int z);
	WorldCoordinateComponent(XMINT3 coord);
	~WorldCoordinateComponent();

	XMINT3& getCoord();

	XMMATRIX& getWorldMatrix();
private:
	XMINT3 coord_;
	XMMATRIX worldMatrix_;
};