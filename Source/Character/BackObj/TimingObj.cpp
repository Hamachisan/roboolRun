#include "TimingObj.h"

//コンストラクタ
TimingObj::TimingObj()
{
	//モデル初期化
	model_ = new Model("Data/Model/backObj/Timing/timing.mdl");

	//スケーリング
	scale_.x = scale_.y = scale_.z = 0.05f;

	//角度調性(ラジアン角で約90度)
	angle_.y = 1.5708;
	invincibleTimer_ = 0.0f;

}

//デストラクタ
TimingObj::~TimingObj()
{
	delete model_;
}

//更新処理
void TimingObj::Update(float elapsedTime)
{
	//速力更新処理
	UpdateVclocity(elapsedTime);
	//オブジェクト行列を更新
	UpdateTransform();
	//モデル行列更新
	model_->UpdateTransform(transform_);
}

//描画処理
void TimingObj::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//死亡した時に呼ばれる(必要ないかも)
void TimingObj::OnDead()
{
	Destroy();
}