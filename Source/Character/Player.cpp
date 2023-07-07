#include "Player.h"
#include "Input/Input.h"
#include <imgui.h>
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Character/Enemys/EnemyManager.h"
#include "Collision.h"
#include "Scene/SceneGame.h"

//コンストラクタ
Player::Player()
{
	model_ = new Model("Data/Model/Player/robo.mdl");

	//モデルが大きいのでスケーリング
	scale_.x = scale_.y = scale_.z = 0.0065f;
	position_.z = START_LINE;
	position_.x = 0.15f;

	//属性を設定
	actors_ = Actor::player;

	//アニメーション設定
	TransitionCountDownState();
	//TransitionMoveState();

	//コントローラー初期化
	ZeroMemory(&vibration_, sizeof(XINPUT_VIBRATION));
	//モーターのスピード
	vibration_.wLeftMotorSpeed = 0;
	vibration_.wRightMotorSpeed = 0;

	//SE
	seDamage_		= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/damage.wav");
	seMove_			= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/move.wav");
	seJump_			= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/jump.wav");
	seAttack_		= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/attack.wav");
	seRoll_			= Audio::Instance().LoadAudioSource("Data/Audio/SE/Player/roll.wav");
	//Effect
	efkJump_		= std::make_unique<Effect>("Data/Effect/HipDrop2.efk");
}

//デストラクタ
Player::~Player()
{
	delete model_;
}

//更新処理
void Player::Update(float elapsedTime)
{
	//ゲームが開始していない時に待機アニメーションを再生する
	if (!isGameStart_)
	{

		//速力更新処理
		UpdateVclocity(elapsedTime);

		//カウントダウンアニメーション再生
		UpdateCountDownState(elapsedTime);
		//アニメーション再生処理
		model_->UpdateAnimation(elapsedTime);

		//オブジェクト行列を更新
		UpdateTransform();

		//モデル行列更新
		model_->UpdateTransform(transform_);	//忘れたらダメ！！

		return;

	}

	//ゲームスタートした時にアニメーションをMoveに変更
	if (transitionMoveStateCount_ == 0)
		TransitionMoveState();
	transitionMoveStateCount_++;


	//加速処理
	timeAccelerationSpeed_ += elapsedTime * 10 * 2.5f;	//2.5掛けてるのは補正
	//最大
	if (timeAccelerationSpeed_ > maxSpeed_)
	{
		timeAccelerationSpeed_ = maxSpeed_;
		isMaxSpeed_ = true;
	}

	nowSpeed_ = timeAccelerationSpeed_ + moveSpeed_;

	//デバック時のみゴールへワープするショートカット
#ifdef _DEBUG
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_ENTER)
	{
		position_.z = GOAL_LINE - 10;
	}
#else
#endif

	//移動入力処理
	InputMove(elapsedTime);
	
	//速力更新処理
	UpdateVclocity(elapsedTime);

	//ステート事のアニメーション切り替え
	SwitchAnimState(elapsedTime);

	//アニメーション再生処理
	model_->UpdateAnimation(elapsedTime);

	//プレイヤーと敵との衝突判定
	CollisionRayCastPlayerVsEnemies();

	//ベクトルでの判定
	CollisionVector(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model_->UpdateTransform(transform_);	//忘れたらダメ！！

	//コントローラーを振動させる
	PadVibration(elapsedTime);

	//カメラを振動させる時間
	if (isCameraShakeFlag_)
	{
		shakeTimer_ -= elapsedTime;
	}
	if (shakeTimer_ < 0)
	{
		isCameraShakeFlag_ = false;
	}

	//タイミング精度描画
	if (isTimingRender_)
	{
		timingRenderTimer_ -= elapsedTime;
	}
	if (timingRenderTimer_ < 0)
	{
		isTimingRender_ = false;
	}

	//ミスなくゴールしたらMAXコンボ数は現在のコンボ数
	if (!isMiss_)
	{
		maxCombo_ = combo_;
	}
	if (isMiss_)
	{
		if (combo_ > maxCombo_)
			maxCombo_ = combo_;
	}

}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model_);
}

//デバッグ用GUI描画
void Player::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiTreeNodeFlags_None))
	{
		//トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Position", &position_.x);

			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle_.x);
			a.y = DirectX::XMConvertToDegrees(angle_.y);
			a.z = DirectX::XMConvertToDegrees(angle_.z);

			//ImGui::InputFloat3("Angle", &angle.x);
			ImGui::InputFloat3("Angle", &a.x);
			angle_.x = DirectX::XMConvertToRadians(a.x);
			angle_.y = DirectX::XMConvertToRadians(a.y);
			angle_.z = DirectX::XMConvertToRadians(a.z);

			//スケール
			ImGui::InputFloat3("Scale", &scale_.x);

		}
		if (ImGui::CollapsingHeader("Velocity", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//速度
			ImGui::InputFloat3("Velocity", &velocity_.x);

		}
		if (ImGui::CollapsingHeader("Vector", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//ベクトル
			ImGui::InputFloat("Vector", &nextEnemyLenght_);
		}
		if (ImGui::CollapsingHeader("Speed", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//moveSpeed
			ImGui::InputFloat("Speed", &moveSpeed_);
			//nowSpeed
			ImGui::InputFloat("nowSpeed", &nowSpeed_);

			//時間加速
			ImGui::InputFloat("TimeAccelerationSpeed", &timeAccelerationSpeed_);
		}
		if (ImGui::CollapsingHeader("Offset", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderFloat("Offset", &offset_, 0.0f, 10.0f);
		}

		if (ImGui::CollapsingHeader("TimingCount", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputInt("maxCombo", &maxCombo_);
			ImGui::InputInt("PERFECT", &perfectCount_);
			ImGui::InputInt("GREAT", &greatCount_);
			ImGui::InputInt("GOOD", &goodCount_);
		}

	}
	ImGui::End();
}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//レイキャスト始点Y軸の座標
	debugRenderer->DrawSphere({ position_.x , position_.y + offset_, position_.z }, 0.5f,
		DirectX::XMFLOAT4(1, 0, 0, 1));

}

//着地した時に呼ばれる
void Player::OnLanding()
{
	jumpCount_ = 0;
	seJump_->Stop();
}

//ステート事にアニメーション切り替え
void Player::SwitchAnimState(float elapsedTime)
{
	switch (animState_)
	{
	case Player::AnimState::State_Move:
		UpdateMoveState(elapsedTime);
		isAnimMove_ = true;
		break;
	case Player::AnimState::State_Brow:
		UpdateBrowState(elapsedTime);
		isAnimMove_ = false;
		break;
	case Player::AnimState::State_Squat:
		UpdateSquatState(elapsedTime);
		isAnimMove_ = false;
		break;
	case Player::AnimState::State_jump:
		UpdateJumpState(elapsedTime);
		isAnimMove_ = false;
		break;
	default:
		break;
	}
}

//移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//入力情報を取得
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//float ax = gamePad.GetAxisLX();
	//float ay = gamePad.GetAxisLY();
	float ay = 1;	//常に上方向にスティックが傾ている状態
	float ax = 0;	//常にスティックが傾いていない状態

	//カメラ方向とスティックの入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはXZ平面に水平なベクトルになるようにする

	//カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLenght = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLenght > 0.0f)
	{
		//単位ベクトル化
		cameraRightX /= cameraRightLenght;
		cameraRightZ /= cameraRightLenght;
	}

	//カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX =  0;//cameraFront.x;
	float cameraFrontZ =  1;//cameraFront.z;
	float cameraFrontLenght = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLenght > 0.0f)
	{
		//単位ベクトル化
		cameraFrontX /= cameraFrontLenght;
		cameraFrontZ /= cameraFrontLenght;
	}

	//スティックの水平入力値をカメラ右方向に反映し、
	cameraRightX = cameraRightX * ax;
	cameraRightZ = cameraRightZ * ax;
	//スティックの垂直入力値をカメラ前方向に反映し、
	cameraFrontX = cameraFrontX * ay;
	cameraFrontZ = cameraFrontZ * ay;

	//進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = cameraRightX + cameraFrontX;
	vec.z = cameraRightZ + cameraFrontZ;

	//vec.x =(cameraRightX * ax) + (cameraFrontX * ay);
	//vec.z =(cameraRightZ * ax) + (cameraFrontZ * ay);

	//Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;
}

//移動入力処理
void Player::InputMove(float elapsedTime)
{
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//DirectX::XMFLOAT3 moveVec = PlayerMoveVec;

	//ここで加速
	Move(moveVec.x, moveVec.z, moveSpeed_ + timeAccelerationSpeed_);
}

//ジャンプ入力処理
bool Player::InputJump()
{
	//ボタン入力でジャンプ(ジャンプ回数制限付き)
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_L ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_B)
	{
		if (jumpCount_ < jumpLimit_)
		{
			++jumpCount_;
			this->Jump(jumpSpeed_);
			seJump_->Play(false);
			return true;
		}
	}
	return false;
}

//回避(レーン変更)
void Player::Avoidance()
{
	constexpr float METERS = 10.0;			//移動できる距離
	constexpr float ACCELERATION = 50.0;	//一時加速力
	constexpr float STAGE_EDGE = 10.0;		//ステージ端

	//ボタン入力で回避
	GamePad& gamePad = Input::Instance().GetGamePad();

	//左回避
	//右に回避中は左回避出来ない
	if (gamePad.GetButtonDown() & GamePad::BTN_A ||
		gamePad.GetButtonDown() & GamePad::BTN_LEFT
		&& !isMoving_)
	{
		//速度がほぼ0で位置がほぼ左端より大きいときのみ
		constexpr float LARGELY_STAGE_LEFT_EDGE = -9.9f;
		if (velocity_.x < 0.1 && position_.x > LARGELY_STAGE_LEFT_EDGE)
		{
			isAvoidanceFlag_ = true;
			isMoving_ = true;
			oldPositionX_ = position_.x;	//回避する前の位置保存
			seMove_->Play(false);
		}
	}
	if (isAvoidanceFlag_)//回避中
	{
		//PlayerMoveVec.x = -1;

		//左に加速
		DirectX::XMFLOAT3 directionFront = GetFront();
		directionFront.x -= ACCELERATION;
		velocity_.x = directionFront.x;

		float difference = oldPositionX_ - position_.x;	//回避する前の位置と現在の位置の差
		//10m移動し終えた時
		if (difference > METERS)
		{
			isMoving_ = false;
			velocity_.x = 0;
			isAvoidanceFlag_ = false;
			seMove_->Stop();
			//PlayerMoveVec.x = 0;
			if (position_.x < -10.0f)
			{
				position_.x = -STAGE_EDGE;	//端に到達
			}
		}
	}


	//左に回避中は右に回避出来ない
	//右回避
	if (gamePad.GetButtonDown() & GamePad::BTN_D ||
		gamePad.GetButtonDown() & GamePad::BTN_RIGHT
		&& !isMoving_)
	{
		//速度がほぼ0で位置がほぼ右端より小さいときのみ
		constexpr float LARGELY_STAGE_RIGHT_EDGE = 9.9;
		if (velocity_.x < 0.1 && position_.x < LARGELY_STAGE_RIGHT_EDGE)
		{
			isMoving_ = true;
			isAvoidanceFlag2_ = true;
			oldPositionX2_ = position_.x;
			seMove_->Play(false);
		}
	}
	if (isAvoidanceFlag2_)	//回避中
	{
		//右に加速
		//PlayerMoveVec.x = 1;
		DirectX::XMFLOAT3 directionFront = GetFront();
		directionFront.x += ACCELERATION;
		velocity_.x = directionFront.x;

		float difference = oldPositionX2_ - position_.x;	//回避する前の位置と現在の位置の差
		if (difference < -METERS)
		{
			isMoving_ = false;
			velocity_.x = 0;
			isAvoidanceFlag2_ = false;
			seMove_->Stop();
			//PlayerMoveVec.x = 0;
			if (position_.x > 10.0f)
			{
				position_.x = STAGE_EDGE;	//端に到達
			}
		}
	}
}

//攻撃動作
void Player::Attack(float elapsedTime)
{
	//プレイヤーのベクトル
	DirectX::XMVECTOR playerVec = DirectX::XMLoadFloat3(&this->position_);
	int enemyCount = EnemyManager::Instance().GetEnemyCount();
	for (int i = 0; i < enemyCount; i++)
	{
		Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
		DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemy->GetPosition());

		//プレイヤー -> エネミーへのベクトル生成
		DirectX::XMVECTOR playerEnemyVec = DirectX::XMVectorSubtract(enemyVec, playerVec);

		//生成したベクトルの距離を算出し保存
		playerEnemyVecLenght_ = DirectX::XMVectorGetX(DirectX::XMVector3Length(playerEnemyVec));

		//距離が近いものだけ判定(エフェクトがバグってしまうので)
		//float range = 10;
		//if (playerEnemyVecLenght < range)
		//{
			//一定の距離内で攻撃したらヒット
		float hitDistanece = 6.0f;
		Actor enemyActor = enemy->GetCaracterActor();
		//壊せるオブジェクトが相手の時のみダメージが入る
		if (enemyActor == Actor::enemySutter)
		{
			//エネミーが生存している時のみ
			if (enemy->isAlive_)
			{

				if (playerEnemyVecLenght_ < hitDistanece)
				{
					float impulsX = (ImpulsDirection()) * 3;		//左右に吹っ飛ばす量を取得
					float magnification = 3.5f;//倍率
					float impulsZ = ImpulsPower(magnification);		//吹っ飛ばすパワー取得
					enemy->AddImpulse({ impulsX, 70.5f, impulsZ });	//衝撃を与える

					enemy->SetImpulseFlag(true);					//吹っ飛んでいる最中というフラグ
					enemy->impuluseTimer_ = 0.0f;					//吹っ飛んでいる時間のタイマー
					enemy->isAlive_ = false;						//もう死んでいるけど表示させる為にまだ生存させる
					enemy->destroyTime_ = 1.0f;						//生存時間を設定する
					//SutterObj.cpp -> BlownAwayで吹っ飛び終わったやつを消去している

					enemy->isEfkHit_ = true;						//吹っ飛ばしたオブジェクトにエフェクトを再生する

					//コントローラーを振動させるフラグとタイマー
					isVibrationFlag_ = true;
					vibrationTimer_ = 0.2f;

					//SE再生
					seAttack_->Play(false);

					//タイミングの制度判定
					float accuracy = 4.5f;
					AttackTimingAccuracy(playerEnemyVecLenght_, accuracy);



				}
			}
		}
		//}
	}
}

//プレイヤーとエネミーの衝突判定 (レイキャスト)
void Player::CollisionRayCastPlayerVsEnemies()
{
	//プレイヤーのベクトル
	DirectX::XMVECTOR playerVec = DirectX::XMLoadFloat3(&this->position_);
	EnemyManager& enemyManager = EnemyManager::Instance();
	//エネミーの数取得
	int enemyCount = EnemyManager::Instance().GetEnemyCount();
	
	//乱数用
	srand((unsigned int)time(NULL));

	for (int i = 0; i < enemyCount; i++)
	{
		//ベクトル生成
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
		DirectX::XMVECTOR playerEnemyVec = DirectX::XMVectorSubtract(enemyVec, playerVec);
		//生成したベクトルの距離を算出
		playerEnemyVecLenght_ = DirectX::XMVectorGetX(DirectX::XMVector3Length(playerEnemyVec));

		//タイミング精度判定に使うベクトル
		DirectX::XMVECTOR pushButtonPosVec = DirectX::XMLoadFloat3(&pushButtonPosition_);
		DirectX::XMVECTOR pushButtonVecEnemyVec = DirectX::XMVectorSubtract(enemyVec, pushButtonPosVec);
		//距離算出
		float pushButtonEnemyVecLenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(pushButtonVecEnemyVec));

		//プレイヤー -> エネミーへのベクトルの長さが10以下のエネミーのみ判定する
		//かつレイを飛ばしたエネミーが生存している時のみ
		float range = 10;
		if (playerEnemyVecLenght_ < range && enemy->isAlive_)
		{
			//レイキャスト
			Actor enemyActor = enemy->GetCaracterActor();
			//障害物が潜り抜ける、ジャンプする、の時にレイキャストの発射位置を変更しぶつかるように
			if (isOffsetChange_ == false)
			{
				switch (enemyActor)
				{
				/*case Actor::enemySutter:
					offset_ = 4.0f;
					break;*/
				case Actor::enemySquat:
					offset_ = 3.0f;	//頭上
					break;
				case Actor::enemyJump:
					offset_ = 1.0f;	//足元
					break;
				}
			}
			//レイの開始位置...足元
			DirectX::XMFLOAT3 start = { position_.x, position_.y + offset_, position_.z };
			//レイの終点位置...移動の位置 
			DirectX::XMFLOAT3 end = { position_.x, position_.y + offset_, position_.z + 1.0f};

			//レイキャストでの敵との判定
			HitResult hit;
			//衝突した時の処理
			if (enemy->RayCast(start, end, hit))
			{
				//前から衝突した時
				if (velocity_.z > 0.0f)
				{
					//エネミーが生存している時のみ
					if (enemy->isAlive_)
					{
						enemy->isAlive_ = false;				//オブジェクト破壊
						enemy->SetImpulseFlag(true);				
						this->SetImpulseFlag(true);				//プレイヤーにも衝撃を与え減速させる

						float impulsX = ImpulsDirection();		//左右に吹っ飛ばす量を取得
						float impulsZ = nowSpeed_ * 1.2f;		//自分のスピードによって吹っ飛ばす距離を調整
						if (impulsZ < 40)
							impulsZ = 40.0f;					//最低吹っ飛びスピード

						enemy->destroyTime_ = 0.5f;						//生存時間設定
						enemy->impuluseTimer_ = 0.0f;					//タイマーを初期値に戻す
						enemy->AddImpulse({ impulsX, 10.0, impulsZ });	//衝撃を与える

						//時間による加速を0にする
						timeAccelerationSpeed_ = 0;
						isMaxSpeed_ = false;

						//コントローラーを振動させるフラグとタイマー
						isVibrationFlag_ = true;
						vibrationTimer_ = 0.6f;

						//カメラシェイクの為のフラグ
						isCameraShakeFlag_ = true;
						shakeTimer_ = 0.2f;
						oldPositionX_ = position_.x;

						//SE再生
						seDamage_->Play(false);

						//コンボをリセット
						combo_ = 0;
						//ミスしたので現在のコンボ数は最大コンボ数では無くなった
						isMiss_ = true;
					}
				}

			}
			//ぶつからなかった時
			else
			{

				//通りすぎた後
				if (position_.z > enemy->GetPosition().z)
				{
					range = 50;
					//ボタンを押した時のベクトルと
					if (pushButtonEnemyVecLenght < range)
					{
						//タイミング表示
						Actor enemyActor = enemy->GetCaracterActor();
						switch (enemyActor)
						{
							//潜り抜ける障害物かつ自身のアニメ―ションが潜りぬけだった時
						case Character::Actor::enemySquat:	//潜り抜ける障害物
							if (animState_ == AnimState::State_Squat)
							{
								float accuracy = 13.0f;	//判定距離
								TimingAccuracy(pushButtonEnemyVecLenght, accuracy);
							}

							break;
							//ジャンプする障害物かつ自身のアニメーションがジャンプだった時
						case Character::Actor::enemyJump:	//ジャンプする障害物
							if (animState_ == AnimState::State_jump)
							{
								float accuracy = 18.0f;	//判定距離
								TimingAccuracy(pushButtonEnemyVecLenght, accuracy);
							}

							break;
						}
					}
				}
			}

			
		}
	}
}

//障害物を通ったかの判定をベクトルで行う
void Player::CollisionVector(float elapsedTime)
{
	//プレイヤーのベクトル
	DirectX::XMVECTOR playerVec = DirectX::XMLoadFloat3(&this->position_);
	//敵の数取得
	int enemyCount = EnemyManager::Instance().GetEnemyCount();
	EnemyManager& enemyManager = EnemyManager::Instance();

	float maxLenght = FLT_MAX;
	for (int i = 0; i < enemyCount; ++i)
	{
		//エネミーのべクトル
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemy->GetPosition());

		//プレイヤー -> エネミーへのベクトル生成
		DirectX::XMVECTOR playerEnemyVec = DirectX::XMVectorSubtract(enemyVec, playerVec);
		
		//生成したベクトルの距離を算出し保存
		playerEnemyVecLenght_ = DirectX::XMVectorGetX(DirectX::XMVector3Length(playerEnemyVec));
		//一番近い敵を保存(ImGui用)
		if (maxLenght > playerEnemyVecLenght_)
		{
			maxLenght = playerEnemyVecLenght_;
			nextEnemyLenght_ = maxLenght;
		}
		
		
		//通りすぎたら消去
		if (enemy->GetPosition().z < position_.z - 2.0f)
		{
			enemy->ApplyDmaege(DAMAGE, INVINCIBLE_TIME);

		}
	}

}

//攻撃成功時のタイミングの精度処理
void Player::AttackTimingAccuracy(float Lenght, float distance)
{
	//タイミング表示
	isTimingRender_ = true;		//描画OK
	timingRenderTimer_ = 0.6;	//描画時間設定
	timing_ = Timing::GREAT;
	if (Lenght < distance)
		timing_ = Timing::PERFECT;

	//コンボ
	combo_++;

	//判定
	switch (timing_)
	{
	case Timing::GREAT:
		greatCount_++;
		break;
	case Timing::PERFECT:
		perfectCount_++;
		break;
	}
}

//その他アクションのタイミング精度処理
void Player::TimingAccuracy(float Lenght, float distance)
{
	isTimingRender_ = true;			//描画OK
	timingRenderTimer_ = 0.6f;		//描画時間設定

	timing_ = Timing::GREAT;
	if (Lenght < distance)
		timing_ = Timing::PERFECT;

	//毎フレームコンボが加算されるのを防ぐ
	if (isCombo_)
	{
		combo_++;	//コンボ加算
		isCombo_ = false;	//一回だけ処理を通す

		switch (timing_)
		{
		case Timing::GREAT:
			greatCount_++;
			break;
		case Timing::PERFECT:
			perfectCount_++;
			break;
		}
	};
}

//左右方向と威力設定
float Player::ImpulsDirection()
{
	int rad = rand() % 2;					//左右どちらに飛ばすか乱数で選択
	float impulsX;							//宣言	
	if (rad == 0)							//radが0なら右、1なら左方向に吹っ飛ばす
		impulsX = 10.0f;					//	
	else
		impulsX = -10.0f;					//

	return impulsX;
}

//吹っ飛ばすパワーを設定
float Player::ImpulsPower(float magnification)
{
	float impuls = nowSpeed_ * magnification;		//自分のスピードによって吹っ飛ばす距離を調整
	
	return impuls;
}

//コントローラを振動させる
void Player::PadVibration(float elapsedTime)
{
	if (isVibrationFlag_)
		vibrationTimer_ -= elapsedTime;
	if (vibrationTimer_ > 0)
	{
		vibration_.wLeftMotorSpeed = 21000; // use any value between 0-65535 here
		vibration_.wRightMotorSpeed = 21000; // use any value between 0-65535 here
		XInputSetState(word_, &vibration_);
	}
	else
	{
		vibration_.wLeftMotorSpeed = 0; // use any value between 0-65535 here
		vibration_.wRightMotorSpeed = 0; // use any value between 0-65535 here
		XInputSetState(word_, &vibration_);
	}
}

////////////////////////////////////////////////////

//クリアフラグ
bool Player::StageClear()
{
	if (position_.z > GOAL_LINE)
		isClear_ = true;
	else
		isClear_ = false;

	return isClear_;
}