#pragma once
using namespace std;
#include "Other/Constant.h"

class EnemyBlock
{
public:
	int num;
};

class EnemyArrangement
{
public:
	EnemyArrangement(int stageNum);
	~EnemyArrangement();

public:
	// 0-> �j��s�\ 1->�p���` 2->�X���C�f�B���O 3->�W�����v 4->�u���Ȃ�
	//�t�@�C���ǂݍ��ׂ݂̈̃f�[�^
	EnemyBlock stage_[ENEMY_LINE][ENEMY_COLUMN];

private:
	enum StageLevel
	{
		//���x��1
		Level1_Stage1 = 0,	//0	
		Level1_Stage2,
		Level1_Stage3,
		Level1_Stage4,
		Level1_Stage5,
		Level1_Stage6,
		Level1_Stage7,
		Level1_Stage8,

		//���x��2
		Level2_Stage1,		//8
		Level2_Stage2,
		Level2_Stage3,
		Level2_Stage4,
		Level2_Stage5,
		Level2_Stage6,
		Level2_Stage7,
		Level2_Stage8,
	};
};

//csv�t�@�C���ǂݍ���
void CsvLoad(EnemyBlock data[][ENEMY_COLUMN], const char* filename);

 