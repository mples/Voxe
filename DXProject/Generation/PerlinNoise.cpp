#include "PerlinNoise.h"
#include "iostream"

PerlinNoise::PerlinNoise(unsigned int seed, unsigned int octaves_num, float amplitude, float freq, float offset) : seed_(seed), octavesNumber_(octaves_num), amplitude_(amplitude), frequency_(freq), offset_(offset) {
	std::mt19937 generator(seed_);
	std::uniform_real_distribution<float> real_distribution;
	auto random = std::bind(real_distribution, generator);

	for (unsigned int i = 0; i < SIZE ; ++i) {
		DirectX::XMFLOAT2 gradient;
		do {
			gradient = DirectX::XMFLOAT2(2 * random() - 1, 2 * random() - 1);
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

float PerlinNoise::eval(float x, float y) {
	int latt_x0 = ((int)std::floor(x)) & MASK;
	int latt_y0 = ((int)std::floor(y)) & MASK;
	int latt_x1 = (latt_x0 + 1) & MASK;
	int latt_y1 = (latt_y0 + 1) & MASK;

	float diff_x = x - (int)std::floor(x);
	float diff_y = y - (int)std::floor(y);

	float smooth_x = smootstep(diff_x);
	float smooth_y = smootstep(diff_y);

	DirectX::XMFLOAT2 grad_x0y0 = gradients_[hash(latt_x0, latt_y0)];
	DirectX::XMFLOAT2 grad_x1y0 = gradients_[hash(latt_x1, latt_y0)];
	DirectX::XMFLOAT2 grad_x0y1 = gradients_[hash(latt_x0, latt_y1)];
	DirectX::XMFLOAT2 grad_x1y1 = gradients_[hash(latt_x1, latt_y1)];

	DirectX::XMFLOAT2 pos_x0y0(diff_x, diff_y);
	DirectX::XMFLOAT2 pos_x1y0(diff_x - 1, diff_y);
	DirectX::XMFLOAT2 pos_x0y1(diff_x, diff_y - 1);
	DirectX::XMFLOAT2 pos_x1y1(diff_x - 1, diff_y - 1);

	//calculate a
	DirectX::XMVECTOR grad_x0y0_vec = DirectX::XMLoadFloat2(&grad_x0y0);

	DirectX::XMVECTOR pos_x0y0_vec = DirectX::XMLoadFloat2(&pos_x0y0);

	float x0_y0_dot;
	DirectX::XMStoreFloat(&x0_y0_dot, DirectX::XMVector2Dot(grad_x0y0_vec, pos_x0y0_vec));

	DirectX::XMVECTOR grad_x1y0_vec = DirectX::XMLoadFloat2(&grad_x1y0);

	DirectX::XMVECTOR pos_x1y0_vec = DirectX::XMLoadFloat2(&pos_x1y0);

	float x1_y0_dot;
	DirectX::XMStoreFloat(&x1_y0_dot, DirectX::XMVector2Dot(grad_x1y0_vec, pos_x1y0_vec));

	float a = lerp(x0_y0_dot, x1_y0_dot, smooth_x);

	//calculate b
	DirectX::XMVECTOR grad_x0y1_vec = DirectX::XMLoadFloat2(&grad_x0y1);

	DirectX::XMVECTOR pos_x0y1_vec = DirectX::XMLoadFloat2(&pos_x0y1);

	float x0_y1_dot;
	DirectX::XMStoreFloat(&x0_y1_dot, DirectX::XMVector2Dot(grad_x0y1_vec, pos_x0y1_vec));

	DirectX::XMVECTOR grad_x1y1_vec = DirectX::XMLoadFloat2(&grad_x1y1);

	DirectX::XMVECTOR pos_x1y1_vec = DirectX::XMLoadFloat2(&pos_x1y1);

	float x1_y1_dot;
	DirectX::XMStoreFloat(&x1_y1_dot, DirectX::XMVector2Dot(grad_x1y1_vec, pos_x1y1_vec));

	float b = lerp(x0_y1_dot, x1_y1_dot, smooth_x);

	float c = lerp(a, b, smooth_y);
	return lerp(a, b, smooth_y);
}

float PerlinNoise::noise(float x, float y) {
	float calc_x = x * frequency_;
	float calc_y = y * frequency_;

	float value = 0.0f;
	for (int i = 0; i < octavesNumber_; i++) {
		float frequency = std::powf(2.0, i);
		float amplitude = std::powf(0.5, i);
		float e = eval(calc_x * frequency, calc_y * frequency);
		value += e * amplitude;
	}

	return (value * amplitude_) + offset_;
}


int PerlinNoise::hash(int x, int y) {
	return permutationTable_[permutationTable_[x] + y];
}


