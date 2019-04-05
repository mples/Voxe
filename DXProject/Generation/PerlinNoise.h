#pragma once
#include <random>
#include <vector>
#include <functional>

#include "Noise.h"

class PerlinNoise : public Noise {
public:
	PerlinNoise(unsigned int seed);
	~PerlinNoise();

	float eval(const DirectX::XMFLOAT2 point) override;
private:
	static const unsigned int SIZE = 256;
	static const unsigned int MASK = SIZE - 1;

	unsigned int seed_;
	std::vector<DirectX::XMFLOAT2> gradients_;
	std::vector<unsigned int> permutationTable_;

	int hash(int x, int y);
	template<typename T = float>
	inline T lerp(const T &lo, const T &hi, const T &t) {
		return lo * (1 - t) + hi * t;
	}

	inline float smootstep(const float v) {
		return v * v * (3 - 2 * v);
	}
};

