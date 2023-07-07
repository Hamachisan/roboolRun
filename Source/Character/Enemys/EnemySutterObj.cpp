#include"EnemySutterObj.h"
#include <imgui.h>
//�R���X�g���N�^
EnemySutterObj::EnemySutterObj()
{
	model_ = new Model("Data/Model/Enemy/Shutter/shutter.mdl");

	//�G�t�F�N�g�ǂݍ���
	efkHit_ = std::make_unique<Effect>("Data/Effect/particlEffect.efk");
	isEfkHit_ = false;

	//���f�����傫���̂ŃX�P�[�����O
	scale_.x = scale_.y = scale_.z = 0.05f;
	angle_.y = 1.5708;

	//���A������ݒ�
	radius_ = 0.5f;
	height_ = 1.0f;

	//������ݒ�
	actors_ = Actor::enemySutter;
}

//�f�X�g���N�^
EnemySutterObj::~EnemySutterObj()
{
	//is_efkHit = false;
	delete model_;
}

//�X�V����
void EnemySutterObj::Update(float elapsedTime)
{
	//���͍X�V����
	UpdateVclocity(elapsedTime);

	//�������
	if (GetImpulseFlag()) BlownAway(elapsedTime);

	//���G���ԍX�V
	UpdateInvincibleTime(elapsedTime);

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���s��X�V
	model_->UpdateTransform(transform_);
}

//�`�揈��
void EnemySutterObj::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//���S�������ɌĂ΂��
void EnemySutterObj::OnDead()
{
	Destroy();	
}

//���C�L���X�g
bool EnemySutterObj::RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model_, hit);
}

//�������ł���Œ�
void EnemySutterObj::BlownAway(float elapsedTime)
{
	//������у^�C�}�[
	impuluseTimer_ += elapsedTime;

	//�p���`�Ő�����΂������̂݃G�t�F�N�g���Đ�����
	constexpr float EFK_SCALE = 8.0f;
	if(isEfkHit_)
		efkHit_->Play(position_, EFK_SCALE);

	//1�b��ɏ���
	if (impuluseTimer_ > destroyTime_)
	{
		//�_���[�W
		//efkHit->Stop();
		isImpulse_ = false;
		ApplyDmaege(DAMAGE, INVINCIBLE_TIME);
	}
}