#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>
#include <stdlib.h>
#include <vector>
#include "Character/Enemys/EnemyArrangement.h"
#include "Character/TitlePlayer.h"
#include "Stage.h"
#include "Camera.h"
#include "CameraController.h"

#define COLUMN (4)	//�s��̗�
#define FILENAME "saveData.dat"
#define DATA_FORMAT (0)		//0...�o�C�i�� 1...�e�L�X�g		�����݃e�L�X�g���[�h�ł̐����͕s��
#define SHOW_RANKING (3)

struct Ranking
{
	float top_ = 0;
	float second_ = 0;
	float third_ = 0;
	float save_ = 0;
};

class SceneClear : public Scene
{
	//public�֐�
public:
	SceneClear(){}
	~SceneClear(){}

	void Initialize() override;
	void SpriteInitialize() override;
	void ParameterInitialize() override;

	void Finalize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	//UI�̕`��
	void UiRender(ID3D11DeviceContext* dc)override;

	//private�֐�
private:
	void SaveLoadFile();

	void RenderNumber(ID3D11DeviceContext* dc, float* size, int digit, DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 scale = { 64, 64 }, float intervalX = 64, float intervalY = 0);

	//�X�v���C�g�̃p�����[�^���X�V����֐�
	void SpriteParameterUpdate(float elapsedTime) override;

	//UI���`�F���W����֐�
	void ChangeUi() override;

	//���f����
private:
	Stage*	stageModel_		= nullptr;
	TitlePlayer* player_	= nullptr;

	CameraController* cameraController_ = nullptr;
	DirectX::XMFLOAT3 target_;

	//Sprite,Audio, Effect
private:
	std::unique_ptr<Sprite> sprNext_[2]				 = {};			//���̃V�[���֗U�����镶��
	std::unique_ptr<Sprite> sprPoint_				= nullptr;		//pt����
	std::unique_ptr<Sprite> sprNumber_				= nullptr;		//�^�C�}�[
	std::unique_ptr<Sprite> sprTimerTen_			= nullptr;		//�^�C�}�[�̏����_
	std::unique_ptr<Sprite> Textsprites_[8]			= {};			//����font

	std::unique_ptr<Sprite> sprBack_				= nullptr;		//�w�i

	std::unique_ptr<Sprite> sprScore_				= nullptr; 		//�X�R�A����
	std::unique_ptr<Sprite> sprPerfect_				= nullptr;		//�p�[�t�F�N�g����
	std::unique_ptr<Sprite> sprGreat_				= nullptr;		//�O���C�g����
	std::unique_ptr<Sprite> sprClearTime_			= nullptr;		//�N���A�^�C������
	std::unique_ptr<Sprite> sprMaxCombo_			= nullptr;		//MAX�R���{����

	//�����L���O�\��
	std::unique_ptr<Sprite> sprRanking_				= nullptr;		//�����L���O����
	std::unique_ptr<Sprite> spr1st_					= nullptr; 		//1st
	std::unique_ptr<Sprite> spr2nd_					= nullptr; 		//2nd
	std::unique_ptr<Sprite> spr3rd_					= nullptr; 		//3rd

	std::unique_ptr<Sprite> sprPushKeyButton_[2] = {};	//�R���g���[���[�ڑ����Ă��邩�ǂ����ŃG���^�[�L�[��B�{�^����\��


	//�����o�ϐ�
private:
	int sceneState_ = 0;

	//�^�C�}�[�֘A
	float clearTime_ = 0.0f;
	float onesPlace_ = 0;				//1�̈�
	float tensPlace_ = 0;				//10�̈�
	float firstDecimalPlace_ = 0;		//�����_��1��
	float texSize_ = 64;					//�e�N�X�`�����炷�l
	float RankingTimeTexPos_[SHOW_RANKING][6];

	float scoreTexPosX_[6];				//�e�N�X�`��X���W
	float clearTimeTexPosX_[3];				//�e�N�X�`��X���W
	float maxComboTexPosX_[2];				//�e�N�X�`��X���W
	float perfectTexPosX_[2];				//�e�N�X�`��X���W
	float greatTexPosX_[2];				//�e�N�X�`��X���W

	//Sprite���W
	DirectX::XMFLOAT2 pos_[12] = {};
	DirectX::XMFLOAT2 scale_[12] = {};


	//���ʔ��\
	/*Ranking ranking;
	Ranking comparison;*/
	FILE* fp_ = nullptr;

	//����̃X�R�A
	float score_ = 0;

	//�\�[�g����p�̔z��
	std::vector<vector<float>> data_ = { MAX_STAGE * MAX_STAGE_LEVEL, vector<float>(COLUMN) };	//��悸�߂����ᑽ�����Ă���
	//�������ݗp�̔z��
	Ranking ranking_[MAX_STAGE * MAX_STAGE_LEVEL];
	int textCount_ = 0;	//�e�L�X�g�ǂݏ����̎��̂ݎg��

	//���̃X�e�[�W��index
	int stage_ = 0;

	//�R���g���[���[��pad���q���ł��邩�ŕ\������摜��ύX
	int keyOrButton_ = 0;


	//enum
private:
	enum
	{
		top = 0,
		second,
		third,
		save
	};

	enum SpritePram
	{
		//����
		word_score,			//0
		word_clearTime,		//1
		word_maxCombo,		//2
		word_perfect,		//3
		word_great,			//4

		//����
		num_score,			//5
		num_clearTime,		//6
		num_maxCombo,		//7
		num_perfect,		//8
		num_great,			//9

		//�����L���O
		num_ranking,		//10

		//py
		word_pt,			//11
	};
	enum
	{
		KEY,
		BUTTON
	};
};

