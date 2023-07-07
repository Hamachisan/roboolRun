#pragma once

#include "Graphics/Shader.h"
#include "Character/Character.h"
#include "Collision.h"

//エネミー
class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy() override{}

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	//破棄
	void Destroy();

	//レイキャスト
	virtual bool RayCast(const DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& hit) = 0;

	//吹っ飛んでいる最中
	virtual void BlownAway(float elapsedTime) = 0;
};