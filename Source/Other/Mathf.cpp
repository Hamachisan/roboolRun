#include "Mathf.h"
#include <stdlib.h>

//���`�⊮
float Mathf::Lerp(float min, float max, float time)
{
	return min * (1.0f - time) + (max * time);
}

//�w��͈͂̃����_���l���v�Z����
float Mathf::RandomRange(float min, float max)
{
	// 0.0�`1.0�̊Ԃ܂ł̃����_���l
	float value = static_cast<float>(rand()) / RAND_MAX;
	// min�`max�܂ł̃����_���l�ɕϊ�
	return min + (max - min) * value;
}