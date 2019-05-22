#include "MaterialComponent.h"

MaterialComponent::MaterialComponent(Texture & tex) : texture_(std::move(tex)) {
}

MaterialComponent::~MaterialComponent() {
}

Texture & MaterialComponent::getTexture() {
	return texture_;
}
