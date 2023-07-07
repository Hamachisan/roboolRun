#pragma once

#include"Graphics/Model.h"
#include"Enemy.h"
#include "Collision.h"
#include "Effect.h"

//�V���b�^�[
class EnemySutterObj : public Enemy
{
public:
	EnemySutterObj();
	~EnemySutterObj() override;	//�p����̉��z�֐����I�[�o�[���C�h����Ƃ���override�L�[���[�h�����邱��

	//�X�V����
	void Update(float elapsedTime) override;

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit) override;

	//�������ł���Œ�
	void BlownAway(float elapsedTime) override;

protected:
	//���S�������ɌĂ΂��
	void OnDead() override;

private:
	Model* model_ = nullptr;

	//effect�Đ��֘A
	std::unique_ptr<Effect> efkHit_ = nullptr;
	//float efkPlayTimer = 1.0f;
};