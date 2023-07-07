#pragma once

#include "Graphics/Sprite.h"
#include <memory>
#include "Input/Input.h"

class Pause
{
public:
	Pause();
	~Pause();
	bool Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc);

	bool GetIsPause() { return isPause_; }

private:
	bool	isPause_		= false;	//�|�[�Y�����ǂ���
	float	pauseTimer_		= 0.0f;		//�|�[�Y�̃^�C�}�[

	std::unique_ptr<Sprite> sprPause_	= nullptr;	//�|�[�Y�̕���
	std::unique_ptr<Sprite> sprBack_	= nullptr;	//�w�i��������������ׂ�Sprite
};