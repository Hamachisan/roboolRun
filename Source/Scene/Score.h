#pragma once

class Score
{
public:
	Score(){}
	~Score(){}

	//唯一のインスタンス取得
	static Score& Instance()
	{
		static Score score_;
		return score_;
	}

	float ResultScore();

	void Init();

private:
	int		maxCombo_		= 0;		//最大コンボ数
	float	clearTime_		= 0;		//クリアタイム
	int		perfectConut_	= 0;		//パーフェクトでオブジェクトに対処した数
	int		greatCount_		= 0;		//グレイト
	int		goodCount_		= 0;		//グッド

	float	result_			= 0;		//結果

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