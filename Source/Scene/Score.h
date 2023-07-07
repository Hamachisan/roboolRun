#pragma once

class Score
{
public:
	Score(){}
	~Score(){}

	//�B��̃C���X�^���X�擾
	static Score& Instance()
	{
		static Score score_;
		return score_;
	}

	float ResultScore();

	void Init();

private:
	int		maxCombo_		= 0;		//�ő�R���{��
	float	clearTime_		= 0;		//�N���A�^�C��
	int		perfectConut_	= 0;		//�p�[�t�F�N�g�ŃI�u�W�F�N�g�ɑΏ�������
	int		greatCount_		= 0;		//�O���C�g
	int		goodCount_		= 0;		//�O�b�h

	float	result_			= 0;		//����

public:
	void SetMaxCombo(int combo) { maxCombo_ = combo; }
	void SetClearTime(float time) { clearTime_ = time; }
	void SetPerfectConut(int perfect) { perfectConut_ = perfect; }
	void SetGreatCount(int great) { greatCount_ = great; }
	void SetGoodCount(int good) { goodCount_ = good; }

	int GetMaxCombo() { return maxCombo_; }
	float GetClearTime() { return clearTime_; }
	int GetPerfectConut() { return perfectConut_; }
	int GetGreatCount() { return greatCount_; }
	int GetGoodCount() { return goodCount_; }
	int GetResult() { return result_; }
};