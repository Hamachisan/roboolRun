#include "GoalObj.h"

//コンストラクタ
GoalObj::GoalObj()
{
	//モデル初期化
	//model = new Model("Data/Model/backObj/Rock/lowRock.mdl");
	model_ = new Model("Data/Model/backObj/Goal/Goal.mdl");

	//モデルのスケーリング
	scale_.x = scale_.y = scale_.z = 0.08f;

	angle_.y = DirectX::XMConvertToRadians(90);

	invincibleTimer_ = 0.0f;

}

//デストラクタ
GoalObj::~GoalObj()
{
	delete model_;
}

//更新処理
void GoalObj::Update(float elapsedTime)
{
	//速力更新処理
	UpdateVclocity(elapsedTime);
	//オブジェクト行列を更新
	UpdateTransform();
	//モデル行列更新
	model_->UpdateTransform(transform_);
}

//描画処理
void GoalObj::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//死亡した時に呼ばれる(必要ないかも)
void GoalObj::OnDead()
{
	Destroy();
}