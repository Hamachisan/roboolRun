#pragma once

#include <DirectXMath.h>
#include "Effect.h"

#define DAMAGE (10)
#define INVINCIBLE_TIME (0.5f)

//キャラクタークラス(基底クラス)
class Character
{
public:
	Character(){}
	virtual ~Character() {}

	//行列更新処理
	void UpdateTransform();

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position_; }
	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position_ = position; }

	//回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle_; }
	//回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle_ = angle; }

	//スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale_; }
	//スケール設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale_ = scale; }

	//半径取得
	float GetRadius() const { return radius_; }

	//地面に接地しているか
	bool IsGround() const { return isGround_; }

	//高さ取得
	float GetHeight() const { return height_; }

	//ダメージを与える
	//bool ApplyDmaege(int damage);
	bool ApplyDmaege(int damage, float invincibleTime);

	//衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	//衝撃のフラグ
	void SetImpulseFlag(bool impulse) { isImpulse_ = impulse; }
	bool GetImpulseFlag() { return isImpulse_; }

private:
	//垂直速力更新処理
	void UpdateVerticalVelocity(float elapsedFrame);

	//垂直移動更新処理
	void UpdateVerticalMove(float elapsedTime);

	//水平速力更新処理
	void UpdateHorizontalVelocity(float elapsedFrame);

	//水平移動更新処理
	void UpdateHorizontalMove(float elapsedTime);

protected:	//メンバ関数
	//移動処理
	//void Move(float elapsedTime, float vx, float vz, float speed);
	void Move(float vx, float vz, float speed);

	//旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);

	//ジャンプ処理
	void Jump(float speed);

	//速度処理更新
	void UpdateVclocity(float elapsedTime);

	//着地した時に呼ばれる
	virtual void OnLanding() {};

	//ダメージを受けた時に呼ばれる
	virtual void OnDamage() {};

	//死亡した時に呼ばれる
	virtual void OnDead() {};

	//無敵時間
	void UpdateInvincibleTime(float elapsedTime);

protected:	//メンバ変数
	//XMMATRIXとかベクター型で変数を作ってはいけない！
	DirectX::XMFLOAT3		position_	= { 0, 0, 0 };
	DirectX::XMFLOAT3		angle_		= { 0, 0, 0 };
	DirectX::XMFLOAT3		scale_		= { 1, 1, 1 };
	DirectX::XMFLOAT4X4		transform_ = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	float radius_ = 0.5f;

	//重力
	float gravity_ = -1.0f;

	DirectX::XMFLOAT3 velocity_ = { 0, 0, 0 };

	//接地フラグ
	bool isGround_ = false;

	//高さ
	float height_ = 2.0f;

	//体力
	int health_ = 5;

	//無敵時間
	float invincibleTimer_ = 1.0f;

	//摩擦
	float friction_ = 0.5f;

	//吹き飛ばし関連
	float acceleration_ = 1.0f;	//加速
	float maxMoveSpeed_ = 5.0f;
	float moveVecX_ = 0.0f;
	float moveVecZ_ = 0.0f;

	//空中制御
	float airControl_ = 0.3f;

	//レイキャスト用
	float stepOffset_ = 1.0f;

	//キャラクターの種類の取得
	enum class Actor
	{
		player,
		enemySutter,
		enemySquat,
		enemyJump,
		enemyImmortal,
		Norn
	};
	Actor actors_ = Actor::Norn;

public:
	//衝撃処理関連
	bool isImpulse_ = false;
	float impuluseTimer_ = 0.0f;
	bool isAlive_ = true;
	float destroyTime_ = 0.5f;

	////effect再生関連
	bool isEfkHit_ = false;

public:
	//キャラクターの種類取得
	Actor GetCaracterActor() { return actors_; }
};
