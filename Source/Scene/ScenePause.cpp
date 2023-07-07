#include "ScenePause.h"

Pause::Pause()
{
	sprPause_ = std::make_unique<Sprite>("Data/Sprite/Scene/Pause/pause.png");
	sprBack_ = std::make_unique<Sprite>("Data/Sprite/Scene/Pause/blackBack.png");
}

Pause::~Pause()
{

}

bool Pause::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	//�|�[�Y����Ȃ��Ƃ�
	if (!isPause_)
	{
		//�Q�[���V�[��...�{�^���������ă|�[�Y��ʂ�UI���\�������
		if (gamePad.GetButtonDown() & GamePad::BTN_SPACE ||
			gamePad.GetButtonDown() & GamePad::BTN_PAD_MENU)
		{
			isPause_ = true;
			return isPause_;
		}
		return false;
	}
	//�|�[�Y�̂Ƃ�
	else
	{
		//�|�[�Y��ʒ��̏���
		//���̃X�e�[�W��������x���߂����蒼��

		//�^�C�g���֖߂�

		//�{�^���������ă|�[�Y������
		if (gamePad.GetButtonDown() & GamePad::BTN_SPACE ||
			gamePad.GetButtonDown() & GamePad::BTN_PAD_MENU)
		{
			isPause_ = false;
			return isPause_;
		}

		return true;
	}



}

void Pause::Render(ID3D11DeviceContext* dc)
{
	//�|�[�Y(����)
	sprPause_->Render(dc,
		1280 * 0.4, 50,			//���W
		256, 128,				//�X�P�[��
		0, 0, 					//�e�N�X�`����ʒu
		256, 128,				//�e�N�X�`���̑傫��
		0,						//�p�x
		1, 1, 1, 1);			//�F(rgb)�s�����x(a)

	//�s�����x���������^�����̔w�i
	sprBack_->Render(dc,		
		0, 0,					//���W
		1280, 720,				//�X�P�[��
		0, 0, 					//�e�N�X�`����ʒu
		1280, 720,				//�e�N�X�`���̑傫��
		0,						//�p�x
		1, 1, 1, 0.2f);			//�F(rgb)�s�����x(a)
}