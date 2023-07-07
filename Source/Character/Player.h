#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"
#include "Audio/Audio.h"
#include <Xinput.h>

#define MAX_SPEED 50

//プレイヤークラス(キャラクターの派生クラス)
class Player : public Character
{
	//public関数
public:
	
	Player();
	~Player() override;

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//デバック用GUI描画
	void DrawDebugGUI();

	//デバッグプリミティブ描画
	void DrawDebugPrimitive();
	//-------------------------------------------------------------------------------------------------------------------


protected:
	//着地した時に呼ばれる
	void OnLanding() override;

	//-------------------------------------------------------------------------------------------------------------------


	//private関数
private:

	//変換
	DirectX::XMFLOAT3 ConvertToIdentityVec(float x, float y, float z)
	{
		DirectX::XMFLOAT3 identityVec = {};
		DirectX::XMStoreFloat3(&identityVec, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z))));
		return identityVec;
	}

	//アニメーション遷移
	void SwitchAnimState(float elapsedTime);

	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;

	//移動入力処理
	void InputMove(float elapsedTime);

	//ジャンプ入力処理
	bool InputJump();

	//回避動作
	void Avoidance();

	//攻撃動作
	void Attack(float elapsedTime);

	//プレイヤーとエネミーの衝突判定 (レイキャスト)
	void CollisionRayCastPlayerVsEnemies();

	//敵の喰らい判定
	void CollisionVector(float elapsedTime);

	//攻撃の精度
	void AttackTimingAccuracy(float Lenght, float distance);
	//ジャンプ、潜り抜けの精度
	void TimingAccuracy(float Lenght, float distance);

	//左右方向どちらかに何ｍ飛ばすかを取得
	float ImpulsDirection();
	//吹っ飛ばすパワーを計算
	float ImpulsPower(float magnification);

	//コントローラー振動
	void PadVibration(float elapsedTime);

	//-------------------------------------------------------------------------------------------------------------------


	//ゲッター、セッター
public:
	//前方向取得
	DirectX::XMFLOAT3 GetFront() { return ConvertToIdentityVec(transform_._31, transform_._32, transform_._33); }

	//コンボ取得
	int GetCombo() { return combo_; }
	int GetMaxCombo() { return maxCombo_; }

	bool GetCameraShakeFlag() { return isCameraShakeFlag_; }
	float GetOldPosition() { return cameraOldPositionX_; }

	float GetNowSpeed() const { return timeAccelerationSpeed_; };
	float GetMaxSpeed() const { return maxSpeed_; }
	float GetIsMaxSpeed() const { return isMaxSpeed_; }

	//プレイヤーのアニメーション状況取得
	int GetPlayerAnimations() { return animState_; }

	//ゲーム開始しているかの状態をセット
	void SetGameStatrtPlayer(bool start) { isGameStart_ = start; }

	//-------------------------------------------------------------------------------------------------------------------

private:
	//アニメーション関連private関数
	void TransitionMoveState();
	void UpdateMoveState(float elapsedTime);	//移動ステート

	void TransitionBrowState();
	void UpdateBrowState(float elapsedTime);	//吹っ飛ばしステート

	void TransitionSquatState();
	void UpdateSquatState(float elapsedTime);	//スクアット(スライディング)ステート

	void TransitionJumpState();
	void UpdateJumpState(float elapsedTime); //ジャンプステート

	void TransitionCountDownState();
	void UpdateCountDownState(float elapsedTime);

	//-------------------------------------------------------------------------------------------------------------------

	//Sprite,Audio,Effect
private:
	//Audio関連
	//SE
	std::unique_ptr<AudioSource> seDamage_;
	std::unique_ptr<AudioSource> seMove_;
	std::unique_ptr<AudioSource> seJump_;
	std::unique_ptr<AudioSource> seAttack_;
	std::unique_ptr<AudioSource> seRoll_;

	//エフェクト関連
	std::unique_ptr<Effect> efkJump_ = nullptr;

	//-------------------------------------------------------------------------------------------------------------------


	//各種パラメータ
private:
	Model* model_ = nullptr;

	//移動スピード
	float moveSpeed_ = 10.0f;
	//プレイヤー移動ベクトル
	DirectX::XMFLOAT3 PlayerMoveVec_ = { 0, 0, 1 };
	//現在のスピード
	float nowSpeed_ = 0.0f;
	//最大スピード
	float maxSpeed_ = MAX_SPEED;
	//最大スピード時描画する為のフラグ
	bool isMaxSpeed_ = false;
	//時間による加速力
	float timeAccelerationSpeed_ = 0;
	//
	float turnSpeed_ = DirectX::XMConvertToRadians(720);

	//ジャンプ
	float jumpSpeed_ = 20.0f;
	int jumpCount_ = 0;
	int jumpLimit_ = 1;

	//レイキャスト発射ｙ位置
	float offset_ = 0.0f;
	bool isOffsetChange_ = false;

	//回避動作
	bool isAvoidanceFlag_ = false;
	bool isAvoidanceFlag2_ = false;
	float oldPositionX_ = 0.0f;
	float oldPositionX2_ = 0.0f;
	bool isMoving_ = false;

	//ベクトルの距離
	float playerEnemyVecLenght_ = 0;
	
	//次のターゲットまでの距離
	float nextEnemyLenght_ = 0;

	//タイミング
	float timingRenderTimer_ = 1.0f;
	//ボタンを押した時のプレイヤーの位置
	DirectX::XMFLOAT3 pushButtonPosition_ = { 0, 0, 0 };

	//コンボ表示
	int combo_ = 0;
	int maxCombo_ = 0;
	bool isCombo_ = false;	//毎フレームコンボが加算されるのを防ぐ変数
	bool isMiss_ = false;	//ミスしたか

	//カメラシェイク
	bool isCameraShakeFlag_ = false;
	float shakeTimer_ = 1.0f;
	float cameraOldPositionX_ = 0;;

	//コントローラー関連
	XINPUT_VIBRATION vibration_;
	WORD word_ = 0;	//繋いでいるコントローラーの数
	bool isVibrationFlag_ = false;
	float vibrationTimer_ = 0.0f;

	//クリアしたか
	bool isClear_ = true;
	//ゲーム開始しているか
	bool isGameStart_ = false;
	//ゲームを開始した後一度だけアニメ―ションをMoveに変更する為の変数
	int transitionMoveStateCount_ = 0;

	//-------------------------------------------------------------------------------------------------------------------


	//Sceneに持っていきたい関数
public:
	bool StageClear();
	
	//Sceneに持っていきたい変数
public:
	int isAnimMove_ = false;

	//タイミング表示
	int timing_ = 0;
	bool isTimingRender_ = false;

	int perfectCount_ = 0;		//パーフェクトでオブジェクトに対処した数
	int greatCount_ = 0;		//グレイト
	int goodCount_ = 0;			//グッド

	//-------------------------------------------------------------------------------------------------------------------


	//アニメーション関連private
private:
	float animFrame_ = 0.0f;	//現在の再生時間
	float blendSeconds = 0.2f;	//補完

	//enum
private:
	//アニメーション状態のステート
	enum AnimState
	{
		State_Move,		//移動
		State_Brow,		//吹っ飛ばし
		State_Squat,	//スクアット
		State_jump,		//ジャンプ
		State_CountDown	//カウントダウン
	};
	//
	int animState_ = AnimState::State_Move;	//現在のアニメーションの状態

	enum Animations
	{
		Anim_Open,
		Anim_Rotation,
		Anim_Idel,
		Anim_RotationEnd,
		Anim_Walk,
		Anim_RotationContine,
		Anim_Close
	};

public:
	//タイミング表示
	enum Timing
	{
		PERFECT,
		GREAT,
		GOOD
	};

	//-------------------------------------------------------------------------------------------------------------------

};