#include "Score.h"

void Score::Init()
{
	maxCombo_ = 0;			//最大コンボ数
	clearTime_ = 0;			//クリアタイム
	perfectConut_ = 0;		//パーフェクトでオブジェクトに対処した数
	greatCount_ = 0;			//グレイト
	goodCount_ = 0;			//グッド

	result_ = 0;	//結果
}

float Score::ResultScore()
{
	
	float time = 100;

	//resutt = clearTime;
	result_ = time - clearTime_;	//早いほど値が大きくなる

	result_ += maxCombo_ * 2 ;
	result_ += (perfectConut_ * 5);
	result_ += (greatCount_ * 3);
	result_ += (goodCount_ * 2);


	return result_;

}