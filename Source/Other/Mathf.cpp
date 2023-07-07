#include "Mathf.h"
#include <stdlib.h>

//線形補完
float Mathf::Lerp(float min, float max, float time)
{
	return min * (1.0f - time) + (max * time);
}

//指定範囲のランダム値を計算する
float Mathf::RandomRange(float min, float max)
{
	// 0.0～1.0の間までのランダム値
	float value = static_cast<float>(rand()) / RAND_MAX;
	// min～maxまでのランダム値に変換
	return min + (max - min) * value;
}