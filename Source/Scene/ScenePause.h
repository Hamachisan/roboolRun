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
	bool	isPause_		= false;	//ポーズ中かどうか
	float	pauseTimer_		= 0.0f;		//ポーズのタイマー

	std::unique_ptr<Sprite> sprPause_	= nullptr;	//ポーズの文字
	std::unique_ptr<Sprite> sprBack_	= nullptr;	//背景を少し黒くする為のSprite
};