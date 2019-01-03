#include "PerlinNoise.h"
#include "glm/ext.hpp"


PerlinNoise::PerlinNoise(unsigned int seed) : seed_(seed) {
	std::mt19937 generator(seed_);
	std::uniform_real_distribution<float> real_distribution;
	auto random = std::bind(real_distribution, generator);

	for (unsigned int i = 0; i < SIZE ; ++i) {
		float grad_len;
		glm::vec2 gradient;
		do {
			gradient = glm::vec2(2 * random() - 1, 2 * random() - 1);
			float a = gradient.x * gradient.x + gradient.y * gradient.y;
		} while ((gradient.x * gradient.x + gradient.y * gradient.y ) > 1);
		gradients_.push_back(gradient);

		permutationTable_.push_back(i);
	}

	std::uniform_int_distribution<unsigned int> int_distribution;
	auto random_int = std::bind(int_distribution, generator);

	for (unsigned int i = 0; i < SIZE; ++i) {
		std::swap(permutationTable_[i], permutationTable_[random_int() & MASK]);
	}
	for (unsigned int i = 0; i < SIZE; ++i) {
		permutationTable_.push_back(permutationTable_[i]);
	}
}


PerlinNoise::~PerlinNoise() {
}

float PerlinNoise::eval(const glm::vec2 p) {
	int latt_x0 = ((int)std::floor(p.x) ) & MASK;
	int latt_y0 = ((int)std::floor(p.y) ) & MASK;

	int latt_x1 = (latt_x0 + 1) & MASK;
	int latt_y1 = (latt_y0 + 1) & MASK;

	float diff_x = p.x - (int)std::floor(p.x);
	float diff_y = p.y - (int)std::floor(p.y);

	float smooth_x = smootstep(p.x);
	float smooth_y = smootstep(p.y);

	glm::vec2 grad_x0y0 = gradients_[hash(latt_x0, latt_y0)];
	glm::vec2 grad_x1y0 = gradients_[hash(latt_x1, latt_y0)];
	glm::vec2 grad_x0y1 = gradients_[hash(latt_x0, latt_y1)];
	glm::vec2 grad_x1y1 = gradients_[hash(latt_x1, latt_y1)];

	glm::vec2 pos_x0y0(diff_x, diff_y);
	glm::vec2 pos_x1y0(diff_x - 1, diff_y);
	glm::vec2 pos_x0y1(diff_x, diff_y - 1);
	glm::vec2 pos_x1y1(diff_x - 1, diff_y - 1);

	float a = lerp(glm::dot(grad_x0y0, pos_x0y0), glm::dot(grad_x1y0, pos_x1y0), smooth_x);
	float b = lerp(glm::dot(grad_x0y1, pos_x0y0), glm::dot(grad_x1y1, pos_x1y1), smooth_x);
	float c = lerp(a, b, smooth_y);
	return lerp(a, b, smooth_y);
}

int PerlinNoise::hash(int x, int y) {
	return permutationTable_[permutationTable_[x] + y];
}


