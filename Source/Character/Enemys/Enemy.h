#pragma once

#include "Graphics/Shader.h"
#include "Character/Character.h"
#include "Collision.h"

//�G�l�~�[
class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy() override{}

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//�f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	//�j��
	void Destroy();

	//���C�L���X�g
	virtual bool RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit) = 0;

	//�������ł���Œ�
	virtual void BlownAway(float elapsedTime) = 0;
};