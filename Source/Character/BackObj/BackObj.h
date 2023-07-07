#pragma once
#include "Character/Character.h"
#include "Graphics/Graphics.h"

//�w�i�I�u�W�F�N�g�N���X(����ɒu�������ŕ`�悾���s��)
class BackObj : public Character
{
public:
	BackObj() {}
	~BackObj() override {}

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//�j��
	void Destroy();

	//�f�o�b�O�v���~�e�B�u�`��
	//virtual void DrawDebugPrimitive();

};
