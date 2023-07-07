#pragma once

#include <DirectXMath.h>
#include "Effect.h"

#define DAMAGE (10)
#define INVINCIBLE_TIME (0.5f)

//�L�����N�^�[�N���X(���N���X)
class Character
{
public:
	Character(){}
	virtual ~Character() {}

	//�s��X�V����
	void UpdateTransform();

	//�ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position_; }
	//�ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position_ = position; }

	//��]�擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle_; }
	//��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle_ = angle; }

	//�X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale_; }
	//�X�P�[���ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale_ = scale; }

	//���a�擾
	float GetRadius() const { return radius_; }

	//�n�ʂɐڒn���Ă��邩
	bool IsGround() const { return isGround_; }

	//�����擾
	float GetHeight() const { return height_; }

	//�_���[�W��^����
	//bool ApplyDmaege(int damage);
	bool ApplyDmaege(int damage, float invincibleTime);

	//�Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	//�Ռ��̃t���O
	void SetImpulseFlag(bool impulse) { isImpulse_ = impulse; }
	bool GetImpulseFlag() { return isImpulse_; }

private:
	//�������͍X�V����
	void UpdateVerticalVelocity(float elapsedFrame);

	//�����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime);

	//�������͍X�V����
	void UpdateHorizontalVelocity(float elapsedFrame);

	//�����ړ��X�V����
	void UpdateHorizontalMove(float elapsedTime);

protected:	//�����o�֐�
	//�ړ�����
	//void Move(float elapsedTime, float vx, float vz, float speed);
	void Move(float vx, float vz, float speed);

	//���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);

	//�W�����v����
	void Jump(float speed);

	//���x�����X�V
	void UpdateVclocity(float elapsedTime);

	//���n�������ɌĂ΂��
	virtual void OnLanding() {};

	//�_���[�W���󂯂����ɌĂ΂��
	virtual void OnDamage() {};

	//���S�������ɌĂ΂��
	virtual void OnDead() {};

	//���G����
	void UpdateInvincibleTime(float elapsedTime);

protected:	//�����o�ϐ�
	//XMMATRIX�Ƃ��x�N�^�[�^�ŕϐ�������Ă͂����Ȃ��I
	DirectX::XMFLOAT3		position_	= { 0, 0, 0 };
	DirectX::XMFLOAT3		angle_		= { 0, 0, 0 };
	DirectX::XMFLOAT3		scale_		= { 1, 1, 1 };
	DirectX::XMFLOAT4X4		transform_ = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	float radius_ = 0.5f;

	//�d��
	float gravity_ = -1.0f;

	DirectX::XMFLOAT3 velocity_ = { 0, 0, 0 };

	//�ڒn�t���O
	bool isGround_ = false;

	//����
	float height_ = 2.0f;

	//�̗�
	int health_ = 5;

	//���G����
	float invincibleTimer_ = 1.0f;

	//���C
	float friction_ = 0.5f;

	//������΂��֘A
	float acceleration_ = 1.0f;	//����
	float maxMoveSpeed_ = 5.0f;
	float moveVecX_ = 0.0f;
	float moveVecZ_ = 0.0f;

	//�󒆐���
	float airControl_ = 0.3f;

	//���C�L���X�g�p
	float stepOffset_ = 1.0f;

	//�L�����N�^�[�̎�ނ̎擾
	enum class Actor
	{
		player,
		enemySutter,
		enemySquat,
		enemyJump,
		enemyImmortal,
		Norn
	};
	Actor actors_ = Actor::Norn;

public:
	//�Ռ������֘A
	bool isImpulse_ = false;
	float impuluseTimer_ = 0.0f;
	bool isAlive_ = true;
	float destroyTime_ = 0.5f;

	////effect�Đ��֘A
	bool isEfkHit_ = false;

public:
	//�L�����N�^�[�̎�ގ擾
	Actor GetCaracterActor() { return actors_; }
};
