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
	// 0-> 破壊不能 1->パンチ 2->スライディング 3->ジャンプ 4->置かない
	//ファイル読み込みの為のデータ
	EnemyBlock stage_[ENEMY_LINE][ENEMY_COLUMN];

private:
	enum StageLevel
	{
		//レベル1
		Level1_Stage1 = 0,	//0	
		Level1_Stage2,
		Level1_Stage3,
		Level1_Stage4,
		Level1_Stage5,
		Level1_Stage6,
		Level1_Stage7,
		Level1_Stage8,

		//レベル2
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

//csvファイル読み込み
void CsvLoad(EnemyBlock data[][ENEMY_COLUMN], const char* filename);

 