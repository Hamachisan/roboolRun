#pragma once

#include "Graphics/Texture.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Character/Player.h"
#include "Character/Enemys/EnemyArrangement.h"
#include "Stage.h"
#include "CameraController.h"
#include "ScenePause.h"
#include"Other/MyFunction.h"
#include"LIght/Light.h"
#include "Audio/Audio.h"
#include "Score.h"

#include "Other/Mathf.h"

// �Q�[���V�[��
//class SceneGame
class SceneGame : public Scene
{
	//�����o�֐�(public)
public:
	SceneGame() {}
	~SceneGame() override {}

	// ������ (�܂Ƃ�)
	void Initialize() override;
	//�p�����[�^�[������
	void ParameterInitialize() override;
	//�X�v���C�g������
	void SpriteInitialize() override;
	//�G���X�e�[�W�ɔz�u
	void EnemySetStage();

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

	//3D��Ԃ̕`��
	void Render3DScene();
	//UI�̕`��
	void UiRender(ID3D11DeviceContext* dc)override;

	//-------------------------------------------------------------------------------------------------------------------

	//�����o�֐�(private)
private:
	//�w�i�I�u�W�F�N�g�𓮂������߂̊֐�
	void MoveBackObject();
	//���C�g�ݒ�
	void SettingLight(ID3D11DeviceContext* dc, RenderContext& rc)override;
	//�v���C���[�̓����ŐF��ւ��郉�C�g�̐ݒ�
	void PlayerSettingLight(RenderContext& rc);

	//�f�o�b�O�p�t���[�J����
	DirectX::XMFLOAT3 GetMoveVec() const;
	void FreeCamera(float elapsedTime);

	//�}���`�X���b�h�ł̏Փˉ��p
	void ThreadCover();

	//�|�[�Y����
	bool PauseProcess(float elapsedTime);

	//�X�v���C�g�̃p�����[�^���X�V����֐�
	void SpriteParameterUpdate(float elapsedTime) override;

	//UI���`�F���W����֐�
	void ChangeUi() override;

	//1�t���[������Update���s���A�J�E���g�_�E�������Ă��鎞�Ƀv���C���[��������悤��
	void OneFrameUpdate(float elapsedTime);

	//-------------------------------------------------------------------------------------------------------------------

	//���f��
private:
	Stage*				stage_					= nullptr;
	Player*				player_					= nullptr;
	EnemyArrangement*	enemyArrangement_		= nullptr;
	CameraController*	cameraController_		= nullptr;
	Pause*				pause_					= nullptr;

	//-------------------------------------------------------------------------------------------------------------------
	
	//Sprite, Audio, Effect
private:
	std::unique_ptr<Texture> sprSkyTexture_		= nullptr;			//�w�i�摜
	std::unique_ptr<Sprite> sprite_				= nullptr;			//�w�i�摜

	std::unique_ptr<Sprite> sprTimer_			= nullptr;			//�^�C�}�[
	std::unique_ptr<Sprite> sprTimerFrame_		= nullptr;			//�^�C�}�[�̘g
	std::unique_ptr<Sprite> sprTimerTen_		= nullptr;			//�^�C�}�[�̏����_
	std::unique_ptr<Sprite> sprGo_				= nullptr;			//GO!!�\��
	std::unique_ptr<Sprite> sprFinish_			= nullptr;			//FINISH!!�\��
	std::unique_ptr<Sprite> Textsprites_[8];							//�t�H���g
	std::unique_ptr<Sprite> sprSpeedMeter_		= nullptr;			//�X�s�[�h���[�^�[
	std::unique_ptr<Sprite> sprSpeedMeterFrame_	= nullptr;			//�X�s�[�h���[�^�[�̘g
	std::unique_ptr<Sprite> sprSpeedMax_		= nullptr;			//�ő�X�s�[�h���`��
	std::unique_ptr<Sprite> sprSpeed_			= nullptr;			//�X�s�[�h(����)
	std::unique_ptr<Sprite> sprImpuls_			= nullptr;			//�Ռ����󂯂����̃_���[�W�\��	

	//�^�C�~���O
	std::unique_ptr<Sprite> sprPerfect_			= nullptr;			//�p�[�t�F�N�g
	std::unique_ptr<Sprite> sprGreat_			= nullptr;			//�O���C�g
	std::unique_ptr<Sprite> sprGood_			= nullptr;			//�O�b�h

	//�R���{
	std::unique_ptr<Sprite> sprCombo_			= nullptr;			//�R���{����
	std::unique_ptr<Sprite> sprComboConut_		= nullptr;			//�R���{�̐���
	std::unique_ptr<Sprite> sprComboFrame_		= nullptr;			//�R���{�̘g

	//������@UI
	std::unique_ptr<Sprite> sprUiFrame_				= nullptr;		//UI��u�����߂̘g
	std::unique_ptr<Sprite> sprUiBack_				= nullptr;		//UI�̔w��
	std::unique_ptr<Sprite> sprUiObjs_[4]			= {};			//��Q���I�u�W�F�N�gUI
	std::unique_ptr<Sprite> sprUiOperations_[5][2]	= {};			//�{�^���A�L�[�{�[�h

	//Audio�֘A
	//BGM
	std::unique_ptr<AudioSource> bgmGame_			= nullptr;		//�Q�[��BGM
	//SE
	std::unique_ptr<AudioSource> seCountDown_		= nullptr;		//�J�E���g�_�E���̉�
	std::unique_ptr<AudioSource> seFinish_			= nullptr;		//�S�[��������


	std::unique_ptr<Sprite> skyboxRenderer_ = nullptr;		//	��`��p�X�v���C�g
	std::unique_ptr<Texture> skyboxTexture_;					//�p�m���}�X�J�C�{�b�N�X

	//-------------------------------------------------------------------------------------------------------------------

	//enum
private:
	enum Operations
	{
		keyboard = 0,
		gamePad
	};
	enum KeyBoard
	{
		Akey = 0,
		Dkey,
		Jkey,
		Kkey,
		Lkey
	};
	enum Pad
	{
		Left = 0,
		Right,
		Xbutton,
		Abutton,
		Bbutton
	};

	//-------------------------------------------------------------------------------------------------------------------
	
	//�����o�ϐ�(private)
private:
	//
	int stageLevel_ = 0;			//�X�e�[�W�ŏ�Q���̕`���ύX
	DirectX::XMFLOAT3 target_;	//�J�����̃^�[�Q�b�g
	//�|�[�Y
	bool isPause_ = false;

	float gameStartTime_ = 0.0f;	//�Q�[�����X�^�[�g����܂ł̎���
	float gameClearTime_ = 0.0f;	//�Q�[�����N���A������̎���

	//�X�v���C�g�̃p�����[�^
	//�^�C�}�[�֘A
	float onesPlace_					= 0;	//1�̈�
	float tensPlace_					= 0;	//10�̈�
	float firstDecimalPlace_			= 0;	//�����_��1��
	float texSize_						= 64;	//�e�N�X�`�����炷�l
	float timerTexPosX_[3]				= {};	//�e�N�X�`��X���W
	float ImpulsTimer_					= 0;
	//�ő�X�s�[�h�������̓_��
	int flashingTime_					= 0;

	//Go!! �|�W�V����
	float GoPosX_ = 0;

	//���[�^�[
	float meterTexX_ = 0;
	float speed_, maxSpeed_, ratio_ = 0;

	//RGB�O���f�[�V����
	float r_, g_, b_, rStart_, gStart_, bStart_, rEnd_, gEnd_, bEnd_ = 0;

	//�����֘A�̃p�����[�^
	DirectX::XMFLOAT4			ambientLightColor_ = {};
	std::unique_ptr<Light>		directionalLight_;
	DirectX::XMFLOAT3			directional_ = { 0, -1, -1 };
	//�|�C���g���C�g
	DirectX::XMFLOAT4			pointLightPosition_{0, 0, 0, 0};
	DirectX::XMFLOAT4			pointLightColor_{1, 1, 1, 1};//��
	float						pointLightRange_ = 0;
	int							animState_ = 0;

	//�f�o�b�N�p�t���[�J����
	DirectX::XMFLOAT3			freeCameraPosition_{0, 0, 0};
	DirectX::XMFLOAT3			freeCameraVelocity_{0, 0, 0};
	const float					freeCameraSpeed_ = 1.0f;
	bool						isFreeCamera_	= false;
	float						ax_ = 0;
	float						ay_ = 0;

	//�^�C�~���O�\��
	int timing_ = 0;

	//�R���{�\��
	float comboTexPosX_[2] = {};

	//�R���g���[���[��pad���q���ł��邩�ŕ\������摜��ύX
	int	whichOperation_ = Operations::keyboard;
	int anyKeyOrButton_[5] = {};

	//SpriteShader���g��Sprite���ǂ����̔��f
	bool isSpriteShader_ = false;
	//�}���`�X���b�h��initialize�ŋ������Ȃ��p(���[�f�B���O��)
	int threadCover_ = 0;

	//�I�t�X�N���[���p�����_�����O�`��^�[�Q�b�g
	std::unique_ptr<RenderTarget> renderTarget_;
	//	�F���␳���
	ColorGradingData	colorGradingData_;
	// ���˃u���[���
	RadialBlurData radialBlurData_;
	//
	bool isSpriteShaderSet_ = true;

	//���`�⊮�������N���X
	Mathf mathf_;

	//���ʉ�����x�����炷�ׂ̕ϐ�
	int seCount_ = 0;

	//Update��1�t���[�������s���p�ϐ�
	int oneFrameCount = 0;
};
