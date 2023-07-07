#pragma once
#include "Enemy.h"
#include "Graphics/Model.h"

class EnemyImmortalObj : public Enemy
{
public:
	EnemyImmortalObj();
	~EnemyImmortalObj();

	//更新処理
	void Update(float elapsedTime) override;
	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//レイキャストによる衝突判定
	bool RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit) override;

	//吹っ飛んでいる最中
	void BlownAway(float elapsedTime) override;

protected:
	//死亡した時に呼ばれる
	void OnDead() override;

private:
	Model* model_ = nullptr;
};