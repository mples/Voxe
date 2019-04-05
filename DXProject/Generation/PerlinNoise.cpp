#include "PerlinNoise.h"
#include "iostream"

PerlinNoise::PerlinNoise(unsigned int seed) : seed_(seed) {
	std::mt19937 generator(seed_);
	std::uniform_real_distribution<float> real_distribution;
	auto random = std::bind(real_distribution, generator);

	for (unsigned int i = 0; i < SIZE ; ++i) {
		//float grad_len;
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

float PerlinNoise::eval(const DirectX::XMFLOAT2 point) {
	int latt_x0 = ((int)std::floor(point.x) ) & MASK;
	int latt_y0 = ((int)std::floor(point.y) ) & MASK;
	int latt_x1 = (latt_x0 + 1) & MASK;
	int latt_y1 = (latt_y0 + 1) & MASK;

	float diff_x = point.x - (int)std::floor(point.x);
	float diff_y = point.y - (int)std::floor(point.y);

	float smooth_x = smootstep(point.x);
	float smooth_y = smootstep(point.y);

	//std::cout << "latt: " << latt_x0 << " " << latt_y0 << " latt1: " << latt_x1 << " " << latt_y1 << std::endl;
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

int PerlinNoise::hash(int x, int y) {
	return permutationTable_[permutationTable_[x] + y];
}


