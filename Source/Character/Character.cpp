#include "Character.h"
#include "Stage.h"

//�s��X�V����
void Character::UpdateTransform()
{
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	//��]�s����쐬
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle_.x, angle_.y, angle_.z);
	//�ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);
	//3�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;
	//�v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform_, W);

}

//�ړ�����
//void Character::Move(float elapsedTime, float vx, float vz, float speed)
void Character::Move(float vx, float vz, float speed)
{
	//speed *= elapsedTime;
	//position.x += vx * speed;	//�x�N�g���̌����Ƒ傫��
	//position.z += vz * speed;

	//�ړ������x�N�g����ݒ�
	moveVecX_ = vx;
	moveVecZ_ = vz;

	//�ő呬�x�ݒ�
	maxMoveSpeed_ = speed;
}

//���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	//�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f)
	{
		return;
	}

	//�i�s�x�N�g����P�ʃx�N�g����
	vx /= length;
	vz /= length;

	//���g�̉�]�l����O���������߂�
	float frontX = sinf(angle_.y);
	float frontZ = cosf(angle_.y);

	////  ���K�����Ă���v�Z����ׂ��I�I  ////

	//��]�p�����߂�ׁA2�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = frontX * vx + frontZ * vz;	//����
	//float dot = vx * frontX + vz + frontZ;	//�v�Z�̏��Ԃ͓��ς̏ꍇ�͂ǂ���ł��ǂ�

	//���ϒl��-1.0 ~ 1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
	//�������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]�p�x�𒲐�����
	float rot = 1.0f - dot;	//�␳�l
	if (rot < speed) speed = rot;

	//���E������s���ׂ�2�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = frontX * vz - frontZ * vx;

	//2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	//���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		angle_.y += speed;
	}
	else
	{
		angle_.y -= speed;
	}
}

//�W�����v����
void Character::Jump(float speed)
{
	//������̗͂�ݒ�
	velocity_.y = speed;
}

//���͍X�V����
void Character::UpdateVclocity(float elapsedTime)
{
	//�o�߃t���[��
	float elapsedFrame = 60.0f * elapsedTime;

	//�������͍X�V����
	UpdateVerticalVelocity(elapsedFrame);
	
	//�������͍X�V����
	UpdateHorizontalVelocity(elapsedFrame);

	//�����ړ��X�V����
	UpdateVerticalMove(elapsedTime);

	//�����ړ��X�V����
	UpdateHorizontalMove(elapsedTime);

}

//�������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	//XZ���ʂ̑��͂���������
	float length = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	if (length > 0.0f)
	{
		//���C��
		float friction = this->friction_ * elapsedFrame;

		//�󒆂ɂ���Ƃ��͖��C�͂����炷
		if (!isGround_) friction *= airControl_;

		//���C�ɂ�鉡�����̌�������
		if (length > friction)
		{
			//�P�ʃx�N�g����
			float vx = velocity_.x / length;
			float vz = velocity_.z / length;

			//����
			velocity_.x -= vx * friction;
			velocity_.z -= vz * friction;
		}
		//�������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
		else
		{
			velocity_.x = 0.0f;
			velocity_.z = 0.0f;
		}
	}

	//XZ���ʂ̑��͂���������
	if (length <= maxMoveSpeed_)
	{
		//�ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
		float moveVecLength = sqrtf(moveVecX_ * moveVecX_ + moveVecZ_ * moveVecZ_);
		if (moveVecLength > 0.0f)
		{
			//������
			float acceleration = this->acceleration_ * elapsedFrame;

			//�󒆂ɂ��鎞�͉����͂����炷
			if (!isGround_) acceleration *= friction_;

			//�ړ��x�N�g���ɂ���������
			velocity_.x += acceleration * moveVecX_;
			velocity_.z += acceleration * moveVecZ_;


			//�ő呬�x����
			float length = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
			if (length > maxMoveSpeed_)
			{
				//�P�ʃx�N�g����
				float vx = velocity_.x / length;
				float vz = velocity_.z / length;

				//����
				velocity_.x = vx * maxMoveSpeed_;
				velocity_.z = vz * maxMoveSpeed_;
			}
		}
	}

	//�ړ��x�N�g�������Z�b�g
	moveVecX_ = 0.0f;
	moveVecZ_ = 0.0f;
}

//�������͍X�V����
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
	//�d�͏���
	velocity_.y += gravity_ * elapsedFrame;
}

//�����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime)
{
	//�������y������ׂɃ��C�L���X�g�ł͒n�ʔ�����s��Ȃ�
#if 1
	////�ړ�����
	position_.y += velocity_.y * elapsedTime;
	
	//�n�ʔ���
	if (position_.y < 0.0f)
	{
		position_.y = 0.0f;
		velocity_.y = 0.0f;
	
		//���n����
		if (!isGround_)
			OnLanding();
		isGround_ = true;
	}
	else
	{
		isGround_ = false;
	}

	//���C�L���X�g�ł̒n�ʔ���
#else
	//���������̈ړ���
	float my = velocity.y * elapsedTime;

	//������
	if (my < 0.0f)
	{
		//���C�̊J�n�ʒu�͑�����菭����
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		//���C�̏I�_�ʒu�͈ړ���̈ʒu
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		//���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		if (Stage::Instance().RayCast(start, end, hit))
		{
			//�n�ʂɐڒn���Ă���
			position.y = hit.position.y;

			//���n����
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;
		}
		else
		{
			//�󒆂ɕ����Ă���
			position.y += my;
			isGround = false;
		}
	}
	//�㏸��
	else if (my > 0.0f)
	{
		position.y += my;
		isGround - false;
	}
#endif
}

//�����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//�ړ�����
	position_.x += velocity_.x * elapsedTime;
	position_.z += velocity_.z * elapsedTime;
}

//�_���[�W��^����
bool Character::ApplyDmaege(int damage, float invincibleTime)
{
	//���G���Ԃ�����Ə������Ȃ�
	if (invincibleTimer_ > 0.0f) return false;

	//�_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage <= 0) return false;

	//���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (health_ <= 0) return false;

	//���G���Ԃ̐ݒ�
	invincibleTimer_ = invincibleTime;
	
	//�_���[�W����
	health_ -= damage;

	//���S�ʒm
	if (health_ <= 0)
	{
		OnDead();
	}
	//�_���[�W�ʒm
	else
	{
		OnDamage();
	}

	//���N��Ԃ��ύX�����ꍇ��true��Ԃ�
	return true;
}

//�Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//���͂ɗ͂�������
	velocity_.x += impulse.x;
	velocity_.y += impulse.y;
	velocity_.z += impulse.z;
}

//���G���ԍX�V
void Character::UpdateInvincibleTime(float elapsedTime)
{
	if (invincibleTimer_ > 0.0f)
	{
		invincibleTimer_ -= elapsedTime;
	}
}