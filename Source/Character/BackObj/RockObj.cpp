#include "RockObj.h"

//�R���X�g���N�^
RockObj::RockObj()
{
	//���f��������
	//model = new Model("Data/Model/backObj/Rock/lowRock.mdl");
	model_ = new Model("Data/Model/backObj/Rock/LowRock5set.mdl");

	//���f���̃X�P�[�����O
	scale_.x = scale_.y = scale_.z = 0.1f;

	angle_.y = DirectX::XMConvertToRadians(180);

	invincibleTimer_ = 0.0f;

}

//�f�X�g���N�^
RockObj::~RockObj()
{
	delete model_;
}

//�X�V����
void RockObj::Update(float elapsedTime)
{
	//���͍X�V����
	UpdateVclocity(elapsedTime);
	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();
	//���f���s��X�V
	model_->UpdateTransform(transform_);
}

//�`�揈��
void RockObj::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//���S�������ɌĂ΂��(�K�v�Ȃ�����)
void RockObj::OnDead()
{
	Destroy();
}