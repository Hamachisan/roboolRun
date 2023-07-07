#include"EnemySutterObj.h"
#include <imgui.h>
//コンストラクタ
EnemySutterObj::EnemySutterObj()
{
	model_ = new Model("Data/Model/Enemy/Shutter/shutter.mdl");

	//エフェクト読み込み
	efkHit_ = std::make_unique<Effect>("Data/Effect/particlEffect.efk");
	isEfkHit_ = false;

	//モデルが大きいのでスケーリング
	scale_.x = scale_.y = scale_.z = 0.05f;
	angle_.y = 1.5708;

	//幅、高さを設定
	radius_ = 0.5f;
	height_ = 1.0f;

	//属性を設定
	actors_ = Actor::enemySutter;
}

//デストラクタ
EnemySutterObj::~EnemySutterObj()
{
	//is_efkHit = false;
	delete model_;
}

//更新処理
void EnemySutterObj::Update(float elapsedTime)
{
	//速力更新処理
	UpdateVclocity(elapsedTime);

	//吹っ飛び
	if (GetImpulseFlag()) BlownAway(elapsedTime);

	//無敵時間更新
	UpdateInvincibleTime(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model_->UpdateTransform(transform_);
}

//描画処理
void EnemySutterObj::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//死亡した時に呼ばれる
void EnemySutterObj::OnDead()
{
	Destroy();	
}

//レイキャスト
bool EnemySutterObj::RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model_, hit);
}

//吹っ飛んでいる最中
void EnemySutterObj::BlownAway(float elapsedTime)
{
	//吹っ飛びタイマー
	impuluseTimer_ += elapsedTime;

	//パンチで吹っ飛ばした時のみエフェクトを再生する
	constexpr float EFK_SCALE = 8.0f;
	if(isEfkHit_)
		efkHit_->Play(position_, EFK_SCALE);

	//1秒後に消去
	if (impuluseTimer_ > destroyTime_)
	{
		//ダメージ
		//efkHit->Stop();
		isImpulse_ = false;
		ApplyDmaege(DAMAGE, INVINCIBLE_TIME);
	}
}