#include "Score.h"

void Score::Init()
{
	maxCombo_ = 0;			//�ő�R���{��
	clearTime_ = 0;			//�N���A�^�C��
	perfectConut_ = 0;		//�p�[�t�F�N�g�ŃI�u�W�F�N�g�ɑΏ�������
	greatCount_ = 0;			//�O���C�g
	goodCount_ = 0;			//�O�b�h

	result_ = 0;	//����
}

float Score::ResultScore()
{
	
	float time = 100;

	//resutt = clearTime;
	result_ = time - clearTime_;	//�����قǒl���傫���Ȃ�

	result_ += maxCombo_ * 2 ;
	result_ += (perfectConut_ * 5);
	result_ += (greatCount_ * 3);
	result_ += (goodCount_ * 2);


	return result_;

}