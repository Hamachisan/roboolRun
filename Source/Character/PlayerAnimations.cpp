#include "Player.h"
#include "Input/Input.h"

//�ړ��X�e�[�g������
void Player::TransitionMoveState()
{
	//�A�j���\�V�����ݒ�
	animState_ = AnimState::State_Move;
	isOffsetChange_ = false;
	model_->PlayAnimation(Animations::Anim_Walk, true, blendSeconds, 1.5f);
}

//�ړ��X�e�[�g�X�V����
void Player::UpdateMoveState(float elapsedTime)
{

	//�U�����͏���
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_J ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_X)
	{
		TransitionBrowState();
	}

	//���Ⴊ�ݓ��͏���
	if(gamePad.GetButtonDown() & GamePad::BTN_K ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_A)
	{
		TransitionSquatState();
	}

	//�W�����v����
	bool jump = InputJump();
	if (jump) {
		TransitionJumpState();
	}
	
	//�����͏���
	Avoidance();
}

//�U���X�e�[�g������
void Player::TransitionBrowState()
{
	//�A�j���[�V�����ݒ�
	animState_ = AnimState::State_Brow;
	model_->PlayAnimation(Animations::Anim_Rotation, false, blendSeconds, 3.5f);
	animFrame_ = 0.0f;
	isOffsetChange_ = false;
	offset_ = 2.0f;
}

//�U���X�e�[�g�X�V����
void Player::UpdateBrowState(float elapsedTime)
{
	//�W�����v����
	bool jump = InputJump();
	if (jump) {
		TransitionJumpState();
	}

	//�U�����莞��
	animFrame_ = model_->GetCurrentAnimationSeconds();
	if(animFrame_ > 0.75f)
		Attack(elapsedTime);
	//�A�j���[�V�������I��������ړ��X�e�[�g�֑J��
	if (!model_->IsPlayAnimation())
	{
		TransitionMoveState();
		seAttack_->Stop();
	}

	//�����͏���
	Avoidance();
}

//���蔲���鏉����
void Player::TransitionSquatState()
{
	//�A�j���\�V�����ݒ�
	animState_ = AnimState::State_Squat;
	model_->PlayAnimation(Animations::Anim_RotationContine, true, blendSeconds, 4.5f);
	animFrame_ = 0.0f;
	isOffsetChange_ = true;
	seRoll_->Play(false);

	pushButtonPosition_.z = position_.z;
	isCombo_ = true;
}

//����ɂ���X�V����
void Player::UpdateSquatState(float elapsedTime)
{
	//���C���΂�Y�ʒu�𒲐�
	offset_ = 1.0f;

	animFrame_ += elapsedTime;

	//�A�j���[�V�������I��������ړ��A�j���[�V�����֑J��
	//if (!model->IsPlayAnimation())
	if(animFrame_ > 0.3f)
	{
		seRoll_->Stop();
		isOffsetChange_ = false;
		TransitionMoveState();
	}

	//�W�����v����
	bool jump = InputJump();
	if (jump) {
		TransitionJumpState();
	}

	//�����͏���
	Avoidance();
}

//�W�����v����������
void Player::TransitionJumpState()
{
	//�A�j���\�V�����ݒ�
	animState_ = AnimState::State_jump;
	model_->PlayAnimation(Animations::Anim_Close, false, blendSeconds, 4.5f);
	animFrame_ = 0.0f;
	isOffsetChange_ = true;

	pushButtonPosition_.z = position_.z;
	isCombo_ = true;

	//�W�����v�������ɂԂ���Ȃ��悤�ɉ��Ń��C�L���X�g�I�t�Z�b�g�l��ύX()
	offset_ = 1.2f;
}

//�W�����v�X�V����
void Player::UpdateJumpState(float elapsedTime)
{
	animFrame_ += elapsedTime;

	if(animFrame_ < 0.10f)
		efkJump_->Play(position_, 2.0f);

	//�U�����͏���
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_J ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_X)
	{
		TransitionBrowState();
	}

	//���Ⴊ�ݓ��͏���
	if (gamePad.GetButtonDown() & GamePad::BTN_K ||
		gamePad.GetButtonDown() & GamePad::BTN_PAD_A)
	{
		TransitionSquatState();
	}

	//�A�j���[�V�������I��������ړ��A�j���[�V�����֑J��
	if (animFrame_ > 0.35f)
	{
		TransitionMoveState();
	}
	//�����͏���
	Avoidance();
}


//�Q�[���J�E���g�_�E�����A�j���[�V����������
void Player::TransitionCountDownState()
{
	//�A�j���\�V�����ݒ�
	animState_ = AnimState::State_CountDown;
	model_->PlayAnimation(Animations::Anim_Open, false, blendSeconds, 1.1f);
}

//�Q�[���J�E���g�_�E�����̃A�j���[�V����
void Player::UpdateCountDownState(float elapsedTime)
{

}