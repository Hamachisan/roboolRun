#include"EnemyJumpObj.h"

//�R���X�g���N�^
EnemyJumpObj::EnemyJumpObj()
{
	//���f���ǂݍ���
	model_ = new Model("Data/Model/Enemy/Jump/jump.mdl");

	//�X�P�[�����O
	scale_.x = scale_.y = scale_.z = 0.05f;
	//�p�x����(���W�A���p�Ŗ�90�x)
	angle_.y = 1.5708;

	//���A������ݒ�
	radius_ = 0.5f;
	height_ = 1.0f;

	//������ݒ�
	actors_ = Actor::enemyJump;
}

//�f�X�g���N�^
EnemyJumpObj::~EnemyJumpObj()
{
	delete model_;
}

//�X�V����
void EnemyJumpObj::Update(float elapsedTime)
{
	//���͍X�V����
	UpdateVclocity(elapsedTime);

	//���G���ԍX�V����
	UpdateInvincibleTime(elapsedTime);

	//�������
	if (GetImpulseFlag()) BlownAway(elapsedTime);

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���s��X�V����
	model_->UpdateTransform(transform_);
}

//�`�揈��
void EnemyJumpObj::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//���S�������ɌĂ΂��
void EnemyJumpObj::OnDead()
{
	Destroy();
}

//���C�L���X�g
bool EnemyJumpObj::RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model_, hit);
}

//�������ł���Œ�
void EnemyJumpObj::BlownAway(float elapsedTime)
{
	//�^�C�}�[
	impuluseTimer_ += elapsedTime;

	//1�b��ɏ���
	if (impuluseTimer_ > destroyTime_)
	{
		//�_���[�W
		ApplyDmaege(DAMAGE, INVINCIBLE_TIME);
	}
}