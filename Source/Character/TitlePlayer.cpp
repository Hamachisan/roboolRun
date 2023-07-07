#include "TitlePlayer.h"

TitlePlayer::TitlePlayer()
{
	//���f���ǂݍ���
	model_ = new Model("Data/Model/Player/robo.mdl");
	//�X�P�[�����O
	scale_.x = scale_.y = scale_.z = 0.0065f;
	angle_.y = DirectX::XMConvertToRadians(135);
	model_->PlayAnimation(Animations::Anim_Open, false);
}

TitlePlayer::~TitlePlayer()
{
	delete model_;
}
void TitlePlayer::Update(float elapsedTime)
{
	
	//��]�J�n
	if (!model_->IsPlayAnimation() && animIndex_ == Anim_Open)
	{
		animIndex_ = Anim_Rotation;
		model_->PlayAnimation(Animations::Anim_Rotation, false);
	}
	//��]
	if (!model_->IsPlayAnimation() && animIndex_ == Anim_Rotation)
	{
		animIndex_ = Anim_RotationContine;
		rotationTime_ = 3.0f;
		model_->PlayAnimation(Animations::Anim_RotationContine, true);
	}
	//��]�p��
	if (animIndex_ == Anim_RotationContine)
	{
		rotationTime_ -= elapsedTime;
		//��]�����߂�
		if (rotationTime_ < 0)
		{
			animIndex_ = Anim_RotationEnd;
			model_->PlayAnimation(Animations::Anim_RotationEnd, false);
		}
	}

	//����
	if (!model_->IsPlayAnimation() && animIndex_ == Anim_RotationEnd)
	{
		animIndex_ = Anim_Close;
		model_->PlayAnimation(Animations::Anim_Close, false);
	}
	//�I�[�v��
	if (!model_->IsPlayAnimation() && animIndex_ == Anim_Close)
	{
		animIndex_ = Anim_Open;
		model_->PlayAnimation(Animations::Anim_Open, false);
	}

	//���͍X�V����
	UpdateVclocity(elapsedTime);

	//�A�j���[�V�����Đ�����
	model_->UpdateAnimation(elapsedTime);
	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();
	//���f���s��X�V
	model_->UpdateTransform(transform_);
}

void TitlePlayer::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}