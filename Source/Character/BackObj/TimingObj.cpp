#include "TimingObj.h"

//�R���X�g���N�^
TimingObj::TimingObj()
{
	//���f��������
	model_ = new Model("Data/Model/backObj/Timing/timing.mdl");

	//�X�P�[�����O
	scale_.x = scale_.y = scale_.z = 0.05f;

	//�p�x����(���W�A���p�Ŗ�90�x)
	angle_.y = 1.5708;
	invincibleTimer_ = 0.0f;

}

//�f�X�g���N�^
TimingObj::~TimingObj()
{
	delete model_;
}

//�X�V����
void TimingObj::Update(float elapsedTime)
{
	//���͍X�V����
	UpdateVclocity(elapsedTime);
	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();
	//���f���s��X�V
	model_->UpdateTransform(transform_);
}

//�`�揈��
void TimingObj::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//���S�������ɌĂ΂��(�K�v�Ȃ�����)
void TimingObj::OnDead()
{
	Destroy();
}