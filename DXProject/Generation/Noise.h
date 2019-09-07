#pragma once

class Noise {
public:
	virtual ~Noise() {} ;
	//virtual float eval(float x, float y) = 0;
	virtual float noise(float x, float y) = 0;
};

