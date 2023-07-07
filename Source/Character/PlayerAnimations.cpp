#include "Player.h"
#include "Input/Input.h"

//移動ステート初期化
void Player::TransitionMoveState()
{
	//アニメ―ション設定
	animState_ = AnimState::State_Move;
	isOffsetChange_ = false;
	model_->PlayAnimation(Animations::Anim_Walk, true, blendSeconds, 1.5f);
}

//移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{

	//攻撃入力処理
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_J ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_X)
	{
		TransitionBrowState();
	}

	//しゃがみ入力処理
	if(gamePad.GetButtonDown() & GamePad::BTN_K ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_A)
	{
		TransitionSquatState();
	}

	//ジャンプ処理
	bool jump = InputJump();
	if (jump) {
		TransitionJumpState();
	}
	
	//回避入力処理
	Avoidance();
}

//攻撃ステート初期化
void Player::TransitionBrowState()
{
	//アニメーション設定
	animState_ = AnimState::State_Brow;
	model_->PlayAnimation(Animations::Anim_Rotation, false, blendSeconds, 3.5f);
	animFrame_ = 0.0f;
	isOffsetChange_ = false;
	offset_ = 2.0f;
}

//攻撃ステート更新処理
void Player::UpdateBrowState(float elapsedTime)
{
	//ジャンプ処理
	bool jump = InputJump();
	if (jump) {
		TransitionJumpState();
	}

	//攻撃判定時間
	animFrame_ = model_->GetCurrentAnimationSeconds();
	if(animFrame_ > 0.75f)
		Attack(elapsedTime);
	//アニメーションが終了したら移動ステートへ遷移
	if (!model_->IsPlayAnimation())
	{
		TransitionMoveState();
		seAttack_->Stop();
	}

	//回避入力処理
	Avoidance();
}

//潜り抜ける初期化
void Player::TransitionSquatState()
{
	//アニメ―ション設定
	animState_ = AnimState::State_Squat;
	model_->PlayAnimation(Animations::Anim_RotationContine, true, blendSeconds, 4.5f);
	animFrame_ = 0.0f;
	isOffsetChange_ = true;
	seRoll_->Play(false);

	pushButtonPosition_.z = position_.z;
	isCombo_ = true;
}

//潜りにける更新処理
void Player::UpdateSquatState(float elapsedTime)
{
	//レイを飛ばすY位置を調整
	offset_ = 1.0f;

	animFrame_ += elapsedTime;

	//アニメーションが終了したら移動アニメーションへ遷移
	//if (!model->IsPlayAnimation())
	if(animFrame_ > 0.3f)
	{
		seRoll_->Stop();
		isOffsetChange_ = false;
		TransitionMoveState();
	}

	//ジャンプ処理
	bool jump = InputJump();
	if (jump) {
		TransitionJumpState();
	}

	//回避入力処理
	Avoidance();
}

//ジャンプ処理初期化
void Player::TransitionJumpState()
{
	//アニメ―ション設定
	animState_ = AnimState::State_jump;
	model_->PlayAnimation(Animations::Anim_Close, false, blendSeconds, 4.5f);
	animFrame_ = 0.0f;
	isOffsetChange_ = true;

	pushButtonPosition_.z = position_.z;
	isCombo_ = true;

	//ジャンプした時にぶつからないように仮でレイキャストオフセット値を変更()
	offset_ = 1.2f;
}

//ジャンプ更新処理
void Player::UpdateJumpState(float elapsedTime)
{
	animFrame_ += elapsedTime;

	if(animFrame_ < 0.10f)
		efkJump_->Play(position_, 2.0f);

	//攻撃入力処理
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_J ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_X)
	{
		TransitionBrowState();
	}

	//しゃがみ入力処理
	if (gamePad.GetButtonDown() & GamePad::BTN_K ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_A)
	{
		TransitionSquatState();
	}

	//アニメーションが終了したら移動アニメーションへ遷移
	if (animFrame_ > 0.35f)
	{
		TransitionMoveState();
	}
	//回避入力処理
	Avoidance();
}


//ゲームカウントダウン時アニメーション初期化
void Player::TransitionCountDownState()
{
	//アニメ―ション設定
	animState_ = AnimState::State_CountDown;
	model_->PlayAnimation(Animations::Anim_Open, false, blendSeconds, 1.1f);
}

//ゲームカウントダウン時のアニメーション
void Player::UpdateCountDownState(float elapsedTime)
{

}