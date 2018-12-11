#pragma once
#include <string>
#include <unordered_map>
enum class ComponentType {
	GraphicComponent,
	CameraComponent,
	InputComponent,
	Invalid
};

inline ComponentType stringToComponent(std::string& name) {
	static std::unordered_map<std::string, ComponentType> types_map = {
		{"GraphicComponent", ComponentType::GraphicComponent},
		{"CameraComponent", ComponentType::CameraComponent},
		{"InputComponent", ComponentType::InputComponent}
	};

	auto found = types_map.find(name);
	if (found != types_map.end()) {
		return (*found).second;
	}
	return ComponentType::Invalid;

}