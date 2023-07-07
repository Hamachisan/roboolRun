#include "GoalObj.h"

//�R���X�g���N�^
GoalObj::GoalObj()
{
	//���f��������
	//model = new Model("Data/Model/backObj/Rock/lowRock.mdl");
	model_ = new Model("Data/Model/backObj/Goal/Goal.mdl");

	//���f���̃X�P�[�����O
	scale_.x = scale_.y = scale_.z = 0.08f;

	angle_.y = DirectX::XMConvertToRadians(90);

	invincibleTimer_ = 0.0f;

}

//�f�X�g���N�^
GoalObj::~GoalObj()
{
	delete model_;
}

//�X�V����
void GoalObj::Update(float elapsedTime)
{
	//���͍X�V����
	UpdateVclocity(elapsedTime);
	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();
	//���f���s��X�V
	model_->UpdateTransform(transform_);
}

//�`�揈��
void GoalObj::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//���S�������ɌĂ΂��(�K�v�Ȃ�����)
void GoalObj::OnDead()
{
	Destroy();
}