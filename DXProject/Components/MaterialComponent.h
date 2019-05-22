#pragma once
#include "../ECS/Component.h"
#include "../Graphics/Texture.h"

class MaterialComponent : public Component<MaterialComponent> {
public:
	MaterialComponent(Texture & tex);
	~MaterialComponent();

	Texture & getTexture();
private:
	Texture texture_;
};