#pragma once

#include"Graphics/Model.h"
#include "Enemy.h"

class EnemyJumpObj : public Enemy
{
public:
	EnemyJumpObj();
	~EnemyJumpObj();

	//�X�V����
	void Update(float elapsedTime) override;
	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//���C�L���X�g�ɂ��Փ˔���
	bool RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit) override;

	//�������ł���Œ�
	void BlownAway(float elapsedTime) override;

protected:
	//���S�������ɌĂ΂��
	void OnDead() override;

private:
	Model* model_ = nullptr;

};