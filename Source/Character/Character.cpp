#include "Character.h"
#include "Stage.h"

//行列更新処理
void Character::UpdateTransform()
{
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	//回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle_.x, angle_.y, angle_.z);
	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);
	//3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform_, W);

}

//移動処理
//void Character::Move(float elapsedTime, float vx, float vz, float speed)
void Character::Move(float vx, float vz, float speed)
{
	//speed *= elapsedTime;
	//position.x += vx * speed;	//ベクトルの向きと大きさ
	//position.z += vz * speed;

	//移動方向ベクトルを設定
	moveVecX_ = vx;
	moveVecZ_ = vz;

	//最大速度設定
	maxMoveSpeed_ = speed;
}

//旋回処理
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	//進行ベクトルがゼロベクトルの場合は処理する必要なし
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f)
	{
		return;
	}

	//進行ベクトルを単位ベクトル化
	vx /= length;
	vz /= length;

	//自身の回転値から前方向を求める
	float frontX = sinf(angle_.y);
	float frontZ = cosf(angle_.y);

	////  正規化してから計算するべし！！  ////

	//回転角を求める為、2つの単位ベクトルの内積を計算する
	float dot = frontX * vx + frontZ * vz;	//内積
	//float dot = vx * frontX + vz + frontZ;	//計算の順番は内積の場合はどちらでも良い

	//内積値は-1.0 ~ 1.0で表現されており、2つの単位ベクトルの角度が
	//小さいほど1.0に近づくという性質を利用して回転角度を調整する
	float rot = 1.0f - dot;	//補正値
	if (rot < speed) speed = rot;

	//左右判定を行う為に2つの単位ベクトルの外積を計算する
	float cross = frontX * vz - frontZ * vx;

	//2Dの外積値が正の場合か負の場合によって左右判定が行える
	//左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		angle_.y += speed;
	}
	else
	{
		angle_.y -= speed;
	}
}

//ジャンプ処理
void Character::Jump(float speed)
{
	//上方向の力を設定
	velocity_.y = speed;
}

//速力更新処理
void Character::UpdateVclocity(float elapsedTime)
{
	//経過フレーム
	float elapsedFrame = 60.0f * elapsedTime;

	//垂直速力更新処理
	UpdateVerticalVelocity(elapsedFrame);
	
	//水平速力更新処理
	UpdateHorizontalVelocity(elapsedFrame);

	//垂直移動更新処理
	UpdateVerticalMove(elapsedTime);

	//水平移動更新処理
	UpdateHorizontalMove(elapsedTime);

}

//水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	//XZ平面の速力を減速する
	float length = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	if (length > 0.0f)
	{
		//摩擦力
		float friction = this->friction_ * elapsedFrame;

		//空中にいるときは摩擦力を減らす
		if (!isGround_) friction *= airControl_;

		//摩擦による横方向の減速処理
		if (length > friction)
		{
			//単位ベクトル化
			float vx = velocity_.x / length;
			float vz = velocity_.z / length;

			//減速
			velocity_.x -= vx * friction;
			velocity_.z -= vz * friction;
		}
		//横方向の速力が摩擦力以下になったので速力を無効化
		else
		{
			velocity_.x = 0.0f;
			velocity_.z = 0.0f;
		}
	}

	//XZ平面の速力を加速する
	if (length <= maxMoveSpeed_)
	{
		//移動ベクトルがゼロベクトルでないなら加速する
		float moveVecLength = sqrtf(moveVecX_ * moveVecX_ + moveVecZ_ * moveVecZ_);
		if (moveVecLength > 0.0f)
		{
			//加速力
			float acceleration = this->acceleration_ * elapsedFrame;

			//空中にいる時は加速力を減らす
			if (!isGround_) acceleration *= friction_;

			//移動ベクトルによる加速処理
			velocity_.x += acceleration * moveVecX_;
			velocity_.z += acceleration * moveVecZ_;


			//最大速度制限
			float length = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
			if (length > maxMoveSpeed_)
			{
				//単位ベクトル化
				float vx = velocity_.x / length;
				float vz = velocity_.z / length;

				//加速
				velocity_.x = vx * maxMoveSpeed_;
				velocity_.z = vz * maxMoveSpeed_;
			}
		}
	}

	//移動ベクトルをリセット
	moveVecX_ = 0.0f;
	moveVecZ_ = 0.0f;
}

//垂直速力更新処理
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
	//重力処理
	velocity_.y += gravity_ * elapsedFrame;
}

//垂直移動更新処理
void Character::UpdateVerticalMove(float elapsedTime)
{
	//処理を軽減する為にレイキャストでは地面判定を行わない
#if 1
	////移動処理
	position_.y += velocity_.y * elapsedTime;
	
	//地面判定
	if (position_.y < 0.0f)
	{
		position_.y = 0.0f;
		velocity_.y = 0.0f;
	
		//着地した
		if (!isGround_)
			OnLanding();
		isGround_ = true;
	}
	else
	{
		isGround_ = false;
	}

	//レイキャストでの地面判定
#else
	//垂直方向の移動量
	float my = velocity.y * elapsedTime;

	//落下中
	if (my < 0.0f)
	{
		//レイの開始位置は足元より少し上
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		//レイの終点位置は移動後の位置
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		//レイキャストによる地面判定
		HitResult hit;
		if (Stage::Instance().RayCast(start, end, hit))
		{
			//地面に接地している
			position.y = hit.position.y;

			//着地した
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;
		}
		else
		{
			//空中に浮いている
			position.y += my;
			isGround = false;
		}
	}
	//上昇中
	else if (my > 0.0f)
	{
		position.y += my;
		isGround - false;
	}
#endif
}

//水平移動更新処理
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//移動処理
	position_.x += velocity_.x * elapsedTime;
	position_.z += velocity_.z * elapsedTime;
}

//ダメージを与える
bool Character::ApplyDmaege(int damage, float invincibleTime)
{
	//無敵時間があると処理しない
	if (invincibleTimer_ > 0.0f) return false;

	//ダメージが0の場合は健康状態を変更する必要がない
	if (damage <= 0) return false;

	//死亡している場合は健康状態を変更しない
	if (health_ <= 0) return false;

	//無敵時間の設定
	invincibleTimer_ = invincibleTime;
	
	//ダメージ処理
	health_ -= damage;

	//死亡通知
	if (health_ <= 0)
	{
		OnDead();
	}
	//ダメージ通知
	else
	{
		OnDamage();
	}

	//健康状態が変更した場合はtrueを返す
	return true;
}

//衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//速力に力を加える
	velocity_.x += impulse.x;
	velocity_.y += impulse.y;
	velocity_.z += impulse.z;
}

//無敵時間更新
void Character::UpdateInvincibleTime(float elapsedTime)
{
	if (invincibleTimer_ > 0.0f)
	{
		invincibleTimer_ -= elapsedTime;
	}
}