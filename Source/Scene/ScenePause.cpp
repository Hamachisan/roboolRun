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
	//ポーズじゃないとき
	if (!isPause_)
	{
		//ゲームシーン...ボタンを押してポーズ画面のUIが表示される
		if (gamePad.GetButtonDown() & GamePad::BTN_SPACE ||
			gamePad.GetButtonDown() & GamePad::BTN_PAD_MENU)
		{
			isPause_ = true;
			return isPause_;
		}
		return false;
	}
	//ポーズのとき
	else
	{
		//ポーズ画面中の処理
		//そのステージをもう一度初めからやり直す

		//タイトルへ戻る

		//ボタンを押してポーズを解除
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
	//ポーズ(文字)
	sprPause_->Render(dc,
		1280 * 0.4, 50,			//座標
		256, 128,				//スケール
		0, 0, 					//テクスチャ基準位置
		256, 128,				//テクスチャの大きさ
		0,						//角度
		1, 1, 1, 1);			//色(rgb)不透明度(a)

	//不透明度を下げた真っ黒の背景
	sprBack_->Render(dc,		
		0, 0,					//座標
		1280, 720,				//スケール
		0, 0, 					//テクスチャ基準位置
		1280, 720,				//テクスチャの大きさ
		0,						//角度
		1, 1, 1, 0.2f);			//色(rgb)不透明度(a)
}