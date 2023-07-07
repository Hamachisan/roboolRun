#include"EnemySquatObj.h"

//コンストラクタ
EnemySquatObj::EnemySquatObj()
{
	//モデル読み込み
	model_ = new Model("Data/Model/Enemy/Squat/squat.mdl");

	//スケーリング
	scale_.x = scale_.y = scale_.z = 0.05f;
	//角度調整(ラジアン角で約90度)
	angle_.y = 1.5708;

	//幅、高さを設定
	radius_ = 0.5f;
	height_ = 1.0f;

	//属性を設定
	actors_ = Actor::enemySquat;
}

//デストラクタ
EnemySquatObj::~EnemySquatObj()
{
	delete model_;
}

//更新処理
void EnemySquatObj::Update(float elapsedTime)
{
	//速力更新処理
	UpdateVclocity(elapsedTime);

	//無敵時間更新
	UpdateInvincibleTime(elapsedTime);

	//吹っ飛び
	if (GetImpulseFlag()) BlownAway(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model_->UpdateTransform(transform_);
}

//描画処理
void EnemySquatObj::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//死亡した際に呼ばれる
void EnemySquatObj::OnDead()
{
	Destroy();
}

//レイキャストによる衝突処理
bool EnemySquatObj::RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model_, hit);
}

//吹っ飛んでいる最中
void EnemySquatObj::BlownAway(float elapsedTime)
{
	//タイマー
	impuluseTimer_ += elapsedTime;

	//1秒後に消去
	if (impuluseTimer_ > destroyTime_)
	{
		//ダメージ
		ApplyDmaege(DAMAGE, INVINCIBLE_TIME);
	}
}