#pragma once
#include <random>
#include <vector>
#include <functional>
#include <DirectXMath.h>
#include "Noise.h"

class PerlinNoise : public Noise {
public:
	PerlinNoise(unsigned int seed, unsigned int octaves_num, float amplitude, float freq, float offset);
	~PerlinNoise();

	float eval(float x, float y) override;
	float noise(float x, float y) override;
private:
	static const unsigned int SIZE = 256;
	static const unsigned int MASK = SIZE - 1;

	//Noise parameters
	unsigned int octavesNumber_;
	float amplitude_;
	float frequency_;
	float offset_;

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

