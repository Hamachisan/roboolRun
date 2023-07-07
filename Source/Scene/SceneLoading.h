#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "SceneGame.h"

#include <Xinput.h>

//���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
	//�R���X�g���N�^�̏������q���X�g�Ŏ��̃V�[����ݒ�
	SceneLoading(Scene* nextScene) : nextScene_(nextScene) {}
	//SceneLoading(){}
	~SceneLoading() override {};

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

private:
	//���[�f�B���O�X���b�h
	static void LoadingThread(SceneLoading* scene);

	//�X�v���C�g�̃p�����[�^���X�V����֐�
	void SpriteParameterUpdate(float elapsedTime) override;

	//UI���`�F���W����֐�
	void ChangeUi() override;

private:
	std::unique_ptr<Sprite> sprLoadingIcon_		= nullptr;		//���[�f�B���O���̂��邮��
	std::unique_ptr<Sprite> sprLoadingKey_		= nullptr;		//�L�[�{�[�h���쎞�̐���
	std::unique_ptr<Sprite> sprLoadingPad_		= nullptr;		//�p�b�h���쎞�̐���
	std::unique_ptr<Sprite> sprNowLoading_		= nullptr;		//���[�f�B���O����
	std::unique_ptr<Sprite> sprStart_			= nullptr;		//�X�^�[�g����
	std::unique_ptr<Sprite> sprLine_			= nullptr;		//����������郉�C��

	std::unique_ptr<Sprite> sprPushKeyButton_[2] = {};			//�R���g���[���[�ڑ����Ă��邩�ǂ����ŃG���^�[�L�[��B�{�^����\��

	///----------------------------------------------------------------------------

private:
	float	angle_			= 0.0f;				//�A�C�R���̊p�x
	float	posX_			= 1280 - 512;		//���[�f�B���O�����������pX���W�ϐ�
	bool	isRender_		= false;			//�X�^�[�g������\������t���O
	int		flashingTime_	= 0;				//�X�^�[�g������_��

	Scene* nextScene_		= nullptr;			//���̃V�[��

	//�R���g���[���[��pad���q���ł��邩�ŕ\������摜��ύX
	bool isController_ = false;

	int keyOrButton = 0;	//0 : �L�[�{�[�h   1 : �p�b�h

	enum
	{
		KEY,
		BUTTON
	};

public:
	//static int nextScenes_;	//���̃V�[��
};