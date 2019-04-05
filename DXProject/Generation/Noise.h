#pragma once
#include <DirectXMath.h>

class Noise {
public:
	virtual ~Noise() {} ;
	virtual float eval(const DirectX::XMFLOAT2 point) = 0;

};

