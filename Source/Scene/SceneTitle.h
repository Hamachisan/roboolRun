#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Stage.h"
#include "Character/Player.h"
#include "Character/TitlePlayer.h"
#include "CameraController.h"
//#include <Xinput.h>
#include "Graphics/Texture.h"
#include "Other/Constant.h"
#include "Audio/Audio.h"

//�^�C�g���V�[��
class SceneTitle : public Scene
{
public:
	SceneTitle() {};
	~SceneTitle() override {}

	//������
	void Initialize() override;
	void SpriteInitialize() override;
	void ParameterInitialize() override;

	//�I����
	void Finalize() override;

	//�X�V����
	void Update(float elapsedTime) override;

	//�`�揈��
	void Render() override;
	//UI�̕`��
	void UiRender(ID3D11DeviceContext* dc)override;

	//-------------------------------------------------------------------------------------------------------------------

private:
	//�X�e�[�W�I��
	void StageSelect();

	//�}���`�X���b�h�ł̏Փˉ��p
	void ThreadCover();

	//�X�v���C�g�̃p�����[�^���X�V����֐�
	void SpriteParameterUpdate(float elapsedTime) override;

	//UI���`�F���W����֐�
	void ChangeUi() override;

	//-------------------------------------------------------------------------------------------------------------------

	//���f��
private:
	Stage* stage_			= nullptr;
	TitlePlayer* player_	= nullptr;

	//-------------------------------------------------------------------------------------------------------------------

	//Sprite
private:
	std::unique_ptr<Sprite> sprGameTitle_			= nullptr;		//�^�C�g��
	std::unique_ptr<Sprite> sprGradationBack_		= nullptr;		//�^�C�g���̌���
	
	std::unique_ptr<Sprite> sprBack_				= nullptr;		//�������𓧉߂�����
	std::unique_ptr<Sprite> sprAllBack_				= nullptr;
	std::unique_ptr<Sprite> sprPushKeyButton_[2]	= {};			//�G���^�[�L�[��������B�{�^���̕\��
	std::unique_ptr<Sprite> sprPush_				= nullptr;		//Push�����̕\��
	std::unique_ptr<Sprite> sprkeyOrButton_[2]		= {};			//Key��Button�������̕\��
	
	std::unique_ptr<Sprite> sprLine_				= nullptr;		//�g

	std::unique_ptr<Sprite> skyboxRenderer_			= nullptr;		//��`��p�X�v���C�g
	std::unique_ptr<Texture> skyboxTexture_;						//�p�m���}�X�J�C�{�b�N�X

	std::unique_ptr<Sprite> sprFont_				= nullptr;		//�t�H���g
	std::unique_ptr<Sprite> sprStage_				= nullptr;		//�X�e�[�W����
	std::unique_ptr<Sprite> sprLevel_				= nullptr;		//���x������

	std::unique_ptr<Sprite> sprArrow_				= nullptr;	//���
	std::unique_ptr<Sprite> sprFrame_				= nullptr;	//�����̎���̈͂�


	//Audio
	std::unique_ptr<AudioSource> bgmTitle_			= nullptr;	//�^�C�g�����BGM
	std::unique_ptr<AudioSource> seSelect_			= nullptr;	//�X�e�[�W�I�����̃s�R�s�R
	std::unique_ptr<AudioSource> seStart_			= nullptr;	//�X�e�[�W�I�����̌��艹



	//-------------------------------------------------------------------------------------------------------------------

private:
	int titleState_ = 0;					//���
	int stageSelect_ = 0;					//�X�e�[�W�I��
	int stageLevelSelect_ = 0;				//�X�e�[�W���x���I��

	//�J����
	CameraController* cameraController_ = nullptr;
	DirectX::XMFLOAT3 target_;

	//SpriteShader���g��Sprite���ǂ����̔��f
	bool isSpriteShader_ = false;
	//�}���`�X���b�h��initialize�ŋ������Ȃ��p(���[�f�B���O��)
	int threadCover_ = 0;

	//�R���g���[���[��pad���q���ł��邩�ŕ\������摜��ύX
	//DWORD dwResult;
	int keyOrButton_ = 0;
	enum
	{
		KEY,
		BUTTON
	};

	///////////////////////////////////////////////////////////////
	//					Sprite�̃p�����[�^
	///////////////////////////////////////////////////////////////

	
	//�L�[or�{�^���Ő؂�ւ��sprite���W�A�X�P�[��
	DirectX::XMFLOAT2 keyOrButtonPos_;
	DirectX::XMFLOAT2 keyOrButtonScale_;

	//Push�̕���sprite���W�B�X�P�[��
	DirectX::XMFLOAT2 pushPos_;
	DirectX::XMFLOAT2 pushScale_;

	//Button��key���̕���sprite�̍��W�A�X�P�[��
	DirectX::XMFLOAT2 StringKeyOrButtonPos_;
	DirectX::XMFLOAT2 StringKeyOrButtonScale_;

	//�^�C�g���ƃ^�C�g���̌��O���f�[�V�����̍��W�A�X�P�[���A�e�N�X�`�����W
	DirectX::XMFLOAT2 titlePos_{};	
	DirectX::XMFLOAT2 titleScale_{};	
	DirectX::XMFLOAT2 titleTexPos_{};
	float posY_ = 0;

	float fontTexPosX_ = 64;
	float fontTexPosX2_ = 64;

	//�X�e�[�W�I����\��
	bool isArrowUp_		= true;
	bool isArrowDown_	= true;
	bool isArrowRight_	= true;
	bool isArrowLeft_	= true;
	//�_�ŗp�^�C�}�[
	int flashingTime_ = 0;
	//se�Đ��t���O
	bool isSeFlag_ = false;
	float sePlayTimer_ = 0.0f;

	///////////////////////////////////////////////////////////////////////

	enum LEVEL
	{
		level1,
		level2
	};
	enum Arrow
	{
		Dwon,
		Up
	};

	int nowStage_ = 0;
	int nowLevel_ = 1;

	float offsetPosY1_ = 480;
	float offsetPosY2_ = 880;


	//�����p�p�����[�^
	float font1PosX_ = 346;
	float font1PosY_ = 530;
	float font2PosX_ = 820;
	float font2PosY_ = 530;
	float Text1PosX_ = 20;
	float Text1PosY_ = 500;
	float Text2PosX_ = 490;
	float Text2PosY_ = 500;

	float arrowLevelPosX_ = 340;
	float arrowLevelPosY1_ = 600;
	float arrowLevelPosY2_ = 460; 

	float arrowStagePosX1_ = 810;
	float arrowStagePosX2_ = 900;
	float arrowStagePosY_ = 530;

	//-------------------------------------------------------------------------------------------------------------------
};