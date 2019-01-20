#pragma once
#include "glm/glm.hpp"

class Noise {
public:
	virtual ~Noise() {} ;
	virtual float eval(const glm::vec2 p) = 0;

};

