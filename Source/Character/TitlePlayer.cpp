#include "TitlePlayer.h"

TitlePlayer::TitlePlayer()
{
	//モデル読み込み
	model_ = new Model("Data/Model/Player/robo.mdl");
	//スケーリング
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
	
	//回転開始
	if (!model_->IsPlayAnimation() && animIndex_ == Anim_Open)
	{
		animIndex_ = Anim_Rotation;
		model_->PlayAnimation(Animations::Anim_Rotation, false);
	}
	//回転
	if (!model_->IsPlayAnimation() && animIndex_ == Anim_Rotation)
	{
		animIndex_ = Anim_RotationContine;
		rotationTime_ = 3.0f;
		model_->PlayAnimation(Animations::Anim_RotationContine, true);
	}
	//回転継続
	if (animIndex_ == Anim_RotationContine)
	{
		rotationTime_ -= elapsedTime;
		//回転を辞める
		if (rotationTime_ < 0)
		{
			animIndex_ = Anim_RotationEnd;
			model_->PlayAnimation(Animations::Anim_RotationEnd, false);
		}
	}

	//閉じる
	if (!model_->IsPlayAnimation() && animIndex_ == Anim_RotationEnd)
	{
		animIndex_ = Anim_Close;
		model_->PlayAnimation(Animations::Anim_Close, false);
	}
	//オープン
	if (!model_->IsPlayAnimation() && animIndex_ == Anim_Close)
	{
		animIndex_ = Anim_Open;
		model_->PlayAnimation(Animations::Anim_Open, false);
	}

	//速力更新処理
	UpdateVclocity(elapsedTime);

	//アニメーション再生処理
	model_->UpdateAnimation(elapsedTime);
	//オブジェクト行列を更新
	UpdateTransform();
	//モデル行列更新
	model_->UpdateTransform(transform_);
}

void TitlePlayer::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}