#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"
#include "Audio/Audio.h"
#include <Xinput.h>

#define MAX_SPEED 50

//�v���C���[�N���X(�L�����N�^�[�̔h���N���X)
class Player : public Character
{
	//public�֐�
public:
	
	Player();
	~Player() override;

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�f�o�b�N�pGUI�`��
	void DrawDebugGUI();

	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();
	//-------------------------------------------------------------------------------------------------------------------


protected:
	//���n�������ɌĂ΂��
	void OnLanding() override;

	//-------------------------------------------------------------------------------------------------------------------


	//private�֐�
private:

	//�ϊ�
	DirectX::XMFLOAT3 ConvertToIdentityVec(float x, float y, float z)
	{
		DirectX::XMFLOAT3 identityVec = {};
		DirectX::XMStoreFloat3(&identityVec, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z))));
		return identityVec;
	}

	//�A�j���[�V�����J��
	void SwitchAnimState(float elapsedTime);

	//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;

	//�ړ����͏���
	void InputMove(float elapsedTime);

	//�W�����v���͏���
	bool InputJump();

	//��𓮍�
	void Avoidance();

	//�U������
	void Attack(float elapsedTime);

	//�v���C���[�ƃG�l�~�[�̏Փ˔��� (���C�L���X�g)
	void CollisionRayCastPlayerVsEnemies();

	//�G�̋�炢����
	void CollisionVector(float elapsedTime);

	//�U���̐��x
	void AttackTimingAccuracy(float Lenght, float distance);
	//�W�����v�A���蔲���̐��x
	void TimingAccuracy(float Lenght, float distance);

	//���E�����ǂ��炩�ɉ�����΂������擾
	float ImpulsDirection();
	//������΂��p���[���v�Z
	float ImpulsPower(float magnification);

	//�R���g���[���[�U��
	void PadVibration(float elapsedTime);

	//-------------------------------------------------------------------------------------------------------------------


	//�Q�b�^�[�A�Z�b�^�[
public:
	//�O�����擾
	DirectX::XMFLOAT3 GetFront() { return ConvertToIdentityVec(transform_._31, transform_._32, transform_._33); }

	//�R���{�擾
	int GetCombo() { return combo_; }
	int GetMaxCombo() { return maxCombo_; }

	bool GetCameraShakeFlag() { return isCameraShakeFlag_; }
	float GetOldPosition() { return cameraOldPositionX_; }

	float GetNowSpeed() const { return timeAccelerationSpeed_; };
	float GetMaxSpeed() const { return maxSpeed_; }
	float GetIsMaxSpeed() const { return isMaxSpeed_; }

	//�v���C���[�̃A�j���[�V�����󋵎擾
	int GetPlayerAnimations() { return animState_; }

	//�Q�[���J�n���Ă��邩�̏�Ԃ��Z�b�g
	void SetGameStatrtPlayer(bool start) { isGameStart_ = start; }

	//-------------------------------------------------------------------------------------------------------------------

private:
	//�A�j���[�V�����֘Aprivate�֐�
	void TransitionMoveState();
	void UpdateMoveState(float elapsedTime);	//�ړ��X�e�[�g

	void TransitionBrowState();
	void UpdateBrowState(float elapsedTime);	//������΂��X�e�[�g

	void TransitionSquatState();
	void UpdateSquatState(float elapsedTime);	//�X�N�A�b�g(�X���C�f�B���O)�X�e�[�g

	void TransitionJumpState();
	void UpdateJumpState(float elapsedTime); //�W�����v�X�e�[�g

	void TransitionCountDownState();
	void UpdateCountDownState(float elapsedTime);

	//-------------------------------------------------------------------------------------------------------------------

	//Sprite,Audio,Effect
private:
	//Audio�֘A
	//SE
	std::unique_ptr<AudioSource> seDamage_;
	std::unique_ptr<AudioSource> seMove_;
	std::unique_ptr<AudioSource> seJump_;
	std::unique_ptr<AudioSource> seAttack_;
	std::unique_ptr<AudioSource> seRoll_;

	//�G�t�F�N�g�֘A
	std::unique_ptr<Effect> efkJump_ = nullptr;

	//-------------------------------------------------------------------------------------------------------------------


	//�e��p�����[�^
private:
	Model* model_ = nullptr;

	//�ړ��X�s�[�h
	float moveSpeed_ = 10.0f;
	//�v���C���[�ړ��x�N�g��
	DirectX::XMFLOAT3 PlayerMoveVec_ = { 0, 0, 1 };
	//���݂̃X�s�[�h
	float nowSpeed_ = 0.0f;
	//�ő�X�s�[�h
	float maxSpeed_ = MAX_SPEED;
	//�ő�X�s�[�h���`�悷��ׂ̃t���O
	bool isMaxSpeed_ = false;
	//���Ԃɂ�������
	float timeAccelerationSpeed_ = 0;
	//
	float turnSpeed_ = DirectX::XMConvertToRadians(720);

	//�W�����v
	float jumpSpeed_ = 20.0f;
	int jumpCount_ = 0;
	int jumpLimit_ = 1;

	//���C�L���X�g���˂��ʒu
	float offset_ = 0.0f;
	bool isOffsetChange_ = false;

	//��𓮍�
	bool isAvoidanceFlag_ = false;
	bool isAvoidanceFlag2_ = false;
	float oldPositionX_ = 0.0f;
	float oldPositionX2_ = 0.0f;
	bool isMoving_ = false;

	//�x�N�g���̋���
	float playerEnemyVecLenght_ = 0;
	
	//���̃^�[�Q�b�g�܂ł̋���
	float nextEnemyLenght_ = 0;

	//�^�C�~���O
	float timingRenderTimer_ = 1.0f;
	//�{�^�������������̃v���C���[�̈ʒu
	DirectX::XMFLOAT3 pushButtonPosition_ = { 0, 0, 0 };

	//�R���{�\��
	int combo_ = 0;
	int maxCombo_ = 0;
	bool isCombo_ = false;	//���t���[���R���{�����Z�����̂�h���ϐ�
	bool isMiss_ = false;	//�~�X������

	//�J�����V�F�C�N
	bool isCameraShakeFlag_ = false;
	float shakeTimer_ = 1.0f;
	float cameraOldPositionX_ = 0;;

	//�R���g���[���[�֘A
	XINPUT_VIBRATION vibration_;
	WORD word_ = 0;	//�q���ł���R���g���[���[�̐�
	bool isVibrationFlag_ = false;
	float vibrationTimer_ = 0.0f;

	//�N���A������
	bool isClear_ = true;
	//�Q�[���J�n���Ă��邩
	bool isGameStart_ = false;
	//�Q�[�����J�n�������x�����A�j���\�V������Move�ɕύX����ׂ̕ϐ�
	int transitionMoveStateCount_ = 0;

	//-------------------------------------------------------------------------------------------------------------------


	//Scene�Ɏ����Ă��������֐�
public:
	bool StageClear();
	
	//Scene�Ɏ����Ă��������ϐ�
public:
	int isAnimMove_ = false;

	//�^�C�~���O�\��
	int timing_ = 0;
	bool isTimingRender_ = false;

	int perfectCount_ = 0;		//�p�[�t�F�N�g�ŃI�u�W�F�N�g�ɑΏ�������
	int greatCount_ = 0;		//�O���C�g
	int goodCount_ = 0;			//�O�b�h

	//-------------------------------------------------------------------------------------------------------------------


	//�A�j���[�V�����֘Aprivate
private:
	float animFrame_ = 0.0f;	//���݂̍Đ�����
	float blendSeconds = 0.2f;	//�⊮

	//enum
private:
	//�A�j���[�V������Ԃ̃X�e�[�g
	enum AnimState
	{
		State_Move,		//�ړ�
		State_Brow,		//������΂�
		State_Squat,	//�X�N�A�b�g
		State_jump,		//�W�����v
		State_CountDown	//�J�E���g�_�E��
	};
	//
	int animState_ = AnimState::State_Move;	//���݂̃A�j���[�V�����̏��

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
	//�^�C�~���O�\��
	enum Timing
	{
		PERFECT,
		GREAT,
		GOOD
	};

	//-------------------------------------------------------------------------------------------------------------------

};