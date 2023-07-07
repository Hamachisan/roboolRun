#pragma once

#include"Graphics/Model.h"
#include"Enemy.h"
#include "Collision.h"
#include "Effect.h"

//シャッター
class EnemySutterObj : public Enemy
{
public:
	EnemySutterObj();
	~EnemySutterObj() override;	//継承先の仮想関数をオーバーライドするときはoverrideキーワードをつけること

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit) override;

	//吹っ飛んでいる最中
	void BlownAway(float elapsedTime) override;

protected:
	//死亡した時に呼ばれる
	void OnDead() override;

private:
	Model* model_ = nullptr;

	//effect再生関連
	std::unique_ptr<Effect> efkHit_ = nullptr;
	//float efkPlayTimer = 1.0f;
};